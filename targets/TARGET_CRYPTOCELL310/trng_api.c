 /*
  *  trng_api.h
  *
  *  Copyright (C) 2017, ARM Limited, All Rights Reserved
  *  SPDX-License-Identifier: Apache-2.0
  *
  *  Licensed under the Apache License, Version 2.0 (the "License"); you may
  *  not use this file except in compliance with the License.
  *  You may obtain a copy of the License at
  *
  *  http://www.apache.org/licenses/LICENSE-2.0
  *
  *  Unless required by applicable law or agreed to in writing, software
  *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
  *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  *  See the License for the specific language governing permissions and
  *  limitations under the License.
  *
  */

#if defined(DEVICE_TRNG)

#include <string.h>
#include "trng_api.h"

extern CRYS_RND_Context_t*   rndContext_ptr;
extern CRYS_RND_WorkBuff_t*  rndWorkBuff_ptr;

void trng_init(trng_t *obj)
{
    CRYS_RND_State_t   *rndState_ptr =  &rndContext_ptr->rndState;


    RNG_PLAT_SetUserRngParameters(rndState_ptr, &obj->trngParams);
}

void trng_free(trng_t *obj)
{
    (void)obj;
}


int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    (void)obj;
    int ret;
    uint32_t entropySizeBits;
    uint32_t  *entrSource_ptr;

    ret = LLF_RND_GetTrngSource(
            &rndContext_ptr->rndState ,    /*in/out*/
                &obj->trngParams,       /*in/out*/
                0,       /*in*/
                &entropySizeBits,  /*in/out*/
                &entrSource_ptr,   /*out*/
                output_length,  /*out*/
                rndWorkBuff_ptr /*in*/);
    if ( ret != 0 )
        return -1;

    memcpy ( output, entrSource_ptr + CRYS_RND_TRNG_SRC_INNER_OFFSET_WORDS , *output_length );
    return 0;
}

#endif //DEVICE_TRNG
