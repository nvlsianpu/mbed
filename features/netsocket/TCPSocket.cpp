/* Socket
 * Copyright (c) 2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "TCPSocket.h"
#include "Timer.h"
#include "mbed_assert.h"

TCPSocket::TCPSocket()
    : _pending(0), _read_sem(0), _write_sem(0),
      _read_in_progress(false), _write_in_progress(false)
{
}

TCPSocket::~TCPSocket()
{
    close();
}

nsapi_protocol_t TCPSocket::get_proto()
{
    return NSAPI_TCP;
}

nsapi_error_t TCPSocket::connect(const SocketAddress &address)
{
    _lock.lock();
    nsapi_error_t ret;

    // If this assert is hit then there are two threads
    // performing a send at the same time which is undefined
    // behavior
    MBED_ASSERT(!_write_in_progress);
    _write_in_progress = true;

    bool blocking_connect_in_progress = false;

    while (true) {
        if (!_socket) {
            ret = NSAPI_ERROR_NO_SOCKET;
            break;
        }

        _pending = 0;
        ret = _stack->socket_connect(_socket, address);
        if ((_timeout == 0) || !(ret == NSAPI_ERROR_IN_PROGRESS || ret == NSAPI_ERROR_ALREADY)) {
            break;
        } else {
            blocking_connect_in_progress = true;

            int32_t count;

            // Release lock before blocking so other threads
            // accessing this object aren't blocked
            _lock.unlock();
            count = _write_sem.wait(_timeout);
            _lock.lock();

            if (count < 1) {
                // Semaphore wait timed out so break out and return
                break;
            }
        }
    }

    _write_in_progress = false;

    /* Non-blocking connect gives "EISCONN" once done - convert to OK for blocking mode if we became connected during this call */
    if (ret == NSAPI_ERROR_IS_CONNECTED && blocking_connect_in_progress) {
        ret = NSAPI_ERROR_OK;
    }

    _lock.unlock();
    return ret;
}

nsapi_error_t TCPSocket::connect(const char *host, uint16_t port)
{
    SocketAddress address;
    nsapi_error_t err = _stack->gethostbyname(host, &address);
    if (err) {
        return NSAPI_ERROR_DNS_FAILURE;
    }

    address.set_port(port);

    // connect is thread safe
    return connect(address);
}

nsapi_size_or_error_t TCPSocket::send(const void *data, nsapi_size_t size)
{
    _lock.lock();
    nsapi_size_or_error_t ret;

    // If this assert is hit then there are two threads
    // performing a send at the same time which is undefined
    // behavior
    MBED_ASSERT(!_write_in_progress);
    _write_in_progress = true;

    while (true) {
        if (!_socket) {
            ret = NSAPI_ERROR_NO_SOCKET;
            break;
        }

        _pending = 0;
        ret = _stack->socket_send(_socket, data, size);
        if ((_timeout == 0) || (ret != NSAPI_ERROR_WOULD_BLOCK)) {
            break;
        } else {
            osStatus_t stat;

            // Release lock before blocking so other threads
            // accessing this object aren't blocked
            _lock.unlock();
            stat = _write_sem.wait(_timeout);
            _lock.lock();

            if (stat != osOK) {
                // Semaphore wait timed out so break out and return
                ret = NSAPI_ERROR_WOULD_BLOCK;
                break;
            }
        }
    }

    _write_in_progress = false;
    _lock.unlock();
    return ret;
}

nsapi_size_or_error_t TCPSocket::recv(void *data, nsapi_size_t size)
{
    _lock.lock();
    nsapi_size_or_error_t ret;

    // If this assert is hit then there are two threads
    // performing a recv at the same time which is undefined
    // behavior
    MBED_ASSERT(!_read_in_progress);
    _read_in_progress = true;

    while (true) {
        if (!_socket) {
            ret = NSAPI_ERROR_NO_SOCKET;
            break;
        }

        _pending = 0;
        ret = _stack->socket_recv(_socket, data, size);
        if ((_timeout == 0) || (ret != NSAPI_ERROR_WOULD_BLOCK)) {
            break;
        } else {
            osStatus_t stat;

            // Release lock before blocking so other threads
            // accessing this object aren't blocked
            _lock.unlock();
            stat = _read_sem.wait(_timeout);
            _lock.lock();

            if (stat != osOK) {
                // Semaphore wait timed out so break out and return
                ret = NSAPI_ERROR_WOULD_BLOCK;
                break;
            }
        }
    }

    _read_in_progress = false;
    _lock.unlock();
    return ret;
}

void TCPSocket::event()
{
    _write_sem.release();
    _read_sem.release();

    _pending += 1;
    if (_callback && _pending == 1) {
        _callback();
    }
}
