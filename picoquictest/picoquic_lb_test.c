/*
* Author: Christian Huitema
* Copyright (c) 2017, Private Octopus, Inc.
* All rights reserved.
*
* Permission to use, copy, modify, and distribute this software for any
* purpose with or without fee is hereby granted, provided that the above
* copyright notice and this permission notice appear in all copies.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL Private Octopus, Inc. BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef _WINDOWS
#include "wincompat.h"
#endif
#include "picoquic_internal.h"
#include "tls_api.h"
#include "picoquic_utils.h"
#include "picotls.h"
#include "picoquic_lb.h"
#include <string.h>
#include "picoquictest_internal.h"

/* Test of the CID generation function.
 */
#define CID_ENCRYPTION_KEY 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16

#define NB_LB_CONFIG_TEST 38

picoquic_load_balancer_config_t cid_for_lb_test_config[NB_LB_CONFIG_TEST] = {
    {
        picoquic_load_balancer_cid_clear,
        3,
        0,
        8,
        0x08,
        0x0123,
        { 0 }
    },
    {
        picoquic_load_balancer_cid_stream_cipher,
        4,
        8,
        13,
        0x8B,
        0x2345,
        { CID_ENCRYPTION_KEY }
    },
    {
        picoquic_load_balancer_cid_block_cipher,
        2,
        0,
        17,
        0x97,
        0x3456,
        { CID_ENCRYPTION_KEY }
    },
    // LB configuration : cr_bits 0x0 length_self_encoding : y sid_len 1
    {
        /* cid 01be sid be su */
        picoquic_load_balancer_cid_clear,
        1,
        0,
        2,
        0x01,
        0xbe,
        { 0 }
    },
    {
        /* cid 0221b7 sid 21 su b7 */
        picoquic_load_balancer_cid_clear,
        1,
        0,
        3,
        0x02,
        0x21,
        { 0 }
    },
    {
        /* cid 03cadfd8 sid ca su dfd8 */
        picoquic_load_balancer_cid_clear,
        1,
        0,
        4,
        0x03,
        0xca,
        { 0 }
    },
    {
        /* cid 041e0c9328 sid 1e su 0c9328 */
        picoquic_load_balancer_cid_clear,
        1,
        0,
        5,
        0x04,
        0x1e,
        { 0 }
    },
    {
        /* cid 050c8f6d9129 sid 0c su 8f6d9129 */
        picoquic_load_balancer_cid_clear,
        1,
        0,
        6,
        0x05,
        0x0c,
        { 0 }
    },
    /* LB configuration : cr_bits 0x0 length_self_encoding : n sid_len 2 */
    {
        /* cid 02aab0 sid aab0 su */
        picoquic_load_balancer_cid_clear,
        2,
        0,
        3,
        0x02,
        0xaab0,
        { 0 }
    },
    {
        /* cid 3ac4b106 sid c4b1 su 06 */
        picoquic_load_balancer_cid_clear,
        2,
        0,
        4,
        0x3a,
        0xc4b1,
        { 0 }
    },
    {
        /* cid 08bd3cf4a0 sid bd3c su f4a0 */
        picoquic_load_balancer_cid_clear,
        2,
        0,
        5,
        0x08,
        0xbd3c,
        { 0 }
    },
    {
        /* cid 3771d59502d6 sid 71d5 su 9502d6 */
        picoquic_load_balancer_cid_clear,
        2,
        0,
        6,
        0x37,
        0x71d5,
        { 0 }
    },
    {
        /* cid 1d57dee8b888f3 sid 57de su e8b888f3 */
        picoquic_load_balancer_cid_clear,
        2,
        0,
        7,
        0x1d,
        0x57de,
        { 0 }
    },
    /* LB configuration : cr_bits 0x0 length_self_encoding : y sid_len 3 */
    {
        /* cid 0336c976 sid 36c976 su */
        picoquic_load_balancer_cid_clear,
        3,
        0,
        4,
        0x03,
        0x36c976,
        { 0 }
    },
    {
        /* cid 04aa291806 sid aa2918 su 06 */
        picoquic_load_balancer_cid_clear,
        3,
        0,
        5,
        0x04,
        0xaa2918,
        { 0 }
    },
    {
        /* cid 0586897bd8b6 sid 86897b su d8b6 */
        picoquic_load_balancer_cid_clear,
        3,
        0,
        6,
        0x05,
        0x86897b,
        { 0 }
    },
    {
        /* cid 063625bcae4de0 sid 3625bc su ae4de0 */
        picoquic_load_balancer_cid_clear,
        3,
        0,
        7,
        0x06,
        0x3625bc,
        { 0 }
    },
    {
        /* cid 07966fb1f3cb535f sid 966fb1 su f3cb535f */
        picoquic_load_balancer_cid_clear,
        3,
        0,
        8,
        0x07,
        0x966fb1,
        { 0 }
    },
    /* LB configuration : cr_bits 0x0 length_self_encoding : n sid_len 4 */
    {
        /* cid 185172fab8 sid 5172fab8 su */
        picoquic_load_balancer_cid_clear,
        4,
        0,
        5,
        0x18,
        0x5172fab8,
        { 0 }
    },
    {
        /* cid 2eb7ff2c9297 sid b7ff2c92 su 97 */
        picoquic_load_balancer_cid_clear,
        4,
        0,
        6,
        0x2e,
        0xb7ff2c92,
        { 0 }
    },
    {
        /* cid 14f3eb3dd3edbe sid f3eb3dd3 su edbe */
        picoquic_load_balancer_cid_clear,
        4,
        0,
        7,
        0x14,
        0xf3eb3dd3,
        { 0 }
    },
    {
        /* cid 3feb31cece744b74 sid eb31cece su 744b74 */
        picoquic_load_balancer_cid_clear,
        4,
        0,
        8,
        0x3f,
        0xeb31cece,
        { 0 }
    },
    {
        /* cid 06b9f34c353ce23bb5 sid b9f34c35 su 3ce23bb5 */
        picoquic_load_balancer_cid_clear,
        4,
        0,
        9,
        0x06,
        0xb9f34c35,
        { 0 }
    },
    /* LB configuration : cr_bits 0x0 length_self_encoding : y sid_len 5 */
    {
        /* cid 05bdcd8d0b1d sid bdcd8d0b1d su */
        picoquic_load_balancer_cid_clear,
        5,
        0,
        6,
        0x05,
        0xbdcd8d0b1d,
        { 0 }
    },
    {
        /* cid 06aee673725a63 sid aee673725a su 63 */
        picoquic_load_balancer_cid_clear,
        5,
        0,
        7,
        0x06,
        0xaee673725a,
        { 0 }
    },
    {
        /* cid 07bbf338ddbf37f4 sid bbf338ddbf su 37f4 */
        picoquic_load_balancer_cid_clear,
        5,
        0,
        8,
        0x07,
        0xbbf338ddbf,
        { 0 }
    },
    {
        /* cid 08fbbca64c26756840 sid fbbca64c26 su 756840 */
        picoquic_load_balancer_cid_clear,
        5,
        0,
        9,
        0x08,
        0xfbbca64c26,
        { 0 }
    },
    {
        /* cid 09e7737c495b93894e34 sid e7737c495b su 93894e34 */
        picoquic_load_balancer_cid_clear,
        5,
        0,
        10,
        0x09,
        0xe7737c495b,
        { 0 }
    },
    /* Test vectors, block cipher */
    /* LB configuration: cr_bits 0x0 length_self_encoding: y sid_len 1
    key 411592e4160268398386af84ea7505d4 */
    {
        /* cid 10564f7c0df399f6d93bdddb1a03886f25 sid 23 su 05231748a80884ed58007847eb9fd0 */
        picoquic_load_balancer_cid_block_cipher,
        1,
        0,
        17,
        0x10,
        0x23,
        { 0x41, 0x15, 0x92, 0xe4, 0x16, 0x02, 0x68, 0x39, 0x83, 0x86, 0xaf, 0x84, 0xea, 0x75, 0x05, 0xd4 }
    },
    {
        /* cid 10d5c03f9dd765d73b3d8610b244f74d02 sid 15 su 76cd6b6f0d3f0b20fc8e633e3a05f3 */
        picoquic_load_balancer_cid_block_cipher,
        1,
        0,
        17,
        0x10,
        0x15,
        { 0x41, 0x15, 0x92, 0xe4, 0x16, 0x02, 0x68, 0x39, 0x83, 0x86, 0xaf, 0x84, 0xea, 0x75, 0x05, 0xd4 }
    },
    {
        /* cid 108ca55228ab23b92845341344a2f956f2 sid 64 su 65c0ce170a9548717498b537cb8790 */
        picoquic_load_balancer_cid_block_cipher,
        1,
        0,
        17,
        0x10,
        0x64,
        { 0x41, 0x15, 0x92, 0xe4, 0x16, 0x02, 0x68, 0x39, 0x83, 0x86, 0xaf, 0x84, 0xea, 0x75, 0x05, 0xd4 }
    },
    {
        /* cid 10e73f3d034aef2f6f501e3a7693d6270a sid 07 su f9ad10c84cc1e89a2492221d74e707 */
        picoquic_load_balancer_cid_block_cipher,
        1,
        0,
        17,
        0x10,
        0x07,
        { 0x41, 0x15, 0x92, 0xe4, 0x16, 0x02, 0x68, 0x39, 0x83, 0x86, 0xaf, 0x84, 0xea, 0x75, 0x05, 0xd4 }
    },
    {
        /* cid 101a6ce13d48b14a77ecfd365595ad2582 sid 6c su 76ce4689b0745b956ef71c2608045d */
        picoquic_load_balancer_cid_block_cipher,
        1,
        0,
        17,
        0x10,
        0x6c,
        { 0x41, 0x15, 0x92, 0xe4, 0x16, 0x02, 0x68, 0x39, 0x83, 0x86, 0xaf, 0x84, 0xea, 0x75, 0x05, 0xd4 }
    },
    /* LB configuration: cr_bits 0x0 length_self_encoding: n sid_len 2
    key 92ce44aecd636aeeff78da691ef48f77 */
    {
        /* cid 20aa09bc65ed52b1ccd29feb7ef995d318 sid a52f su 99278b92a86694ff0ecd64bc2f73 */
        picoquic_load_balancer_cid_block_cipher,
        2,
        0,
        17,
        0x20,
        0xa52f,
        { 0x92, 0xce, 0x44, 0xae, 0xcd, 0x63, 0x6a, 0xee, 0xff, 0x78, 0xda, 0x69, 0x1e, 0xf4, 0x8f, 0x77 }
    },
    {
        /* cid 30b8dbef657bd78a2f870e93f9485d5211 sid 6c49 su 7381c8657a388b4e9594297afe96 */
        picoquic_load_balancer_cid_block_cipher,
        2,
        0,
        17,
        0x30,
        0x6c49,
        { 0x92, 0xce, 0x44, 0xae, 0xcd, 0x63, 0x6a, 0xee, 0xff, 0x78, 0xda, 0x69, 0x1e, 0xf4, 0x8f, 0x77 }
    },
    {
        /* cid 043a8137331eacd2e78383279b202b9a6d sid 4188 su 5ac4b0e0b95f4e7473b49ee2d0dd */
        picoquic_load_balancer_cid_block_cipher,
        2,
        0,
        17,
        0x04,
        0x4188,
        { 0x92, 0xce, 0x44, 0xae, 0xcd, 0x63, 0x6a, 0xee, 0xff, 0x78, 0xda, 0x69, 0x1e, 0xf4, 0x8f, 0x77 }
    },
    {
        /* cid 3ba71ea2bcf0ab95719ab59d3d7fde770d sid 8ccc su 08728807605db25f2ca88be08e0f */
        picoquic_load_balancer_cid_block_cipher,
        2,
        0,
        17,
        0x3b,
        0x8ccc,
        { 0x92, 0xce, 0x44, 0xae, 0xcd, 0x63, 0x6a, 0xee, 0xff, 0x78, 0xda, 0x69, 0x1e, 0xf4, 0x8f, 0x77 }
    },
    {
        /* cid 37ef1956b4ec354f40dc68336a23d42b31 sid c89d su 5a3ccd1471caa0de221ad6c185c0 */
        picoquic_load_balancer_cid_block_cipher,
        2,
        0,
        17,
        0x37,
        0xc89d,
        { 0x92, 0xce, 0x44, 0xae, 0xcd, 0x63, 0x6a, 0xee, 0xff, 0x78, 0xda, 0x69, 0x1e, 0xf4, 0x8f, 0x77 }
    },
    /* LB configuration: cr_bits 0x0 length_self_encoding: y sid_len 3
    key 5c49cb9265efe8ae7b1d3886948b0a34 */

/* cid 10efcffc161d232d113998a49b1dbc4aa0 sid 0690b3 su 958fc9f38fe61b83881b2c5780 */
/* cid 10fc13bdbcb414ba90e391833400c19505 sid 031ac3 su 9a55e1e1904e780346fcc32c3c */
/* cid 10d3cc1efaf5dc52c7a0f6da2746a8c714 sid 572d3a su ff2ec9712664e7174dc03ca3f8 */
/* cid 107edf37f6788e33c0ec7758a485215f2b sid 562c25 su 02c5a5dcbea629c3840da5f567 */
/* cid 10bc28da122582b7312e65aa096e9724fc sid 2fa4f0 su 8ae8c666bfc0fc364ebfd06b9a */

/* LB configuration: cr_bits 0x0 length_self_encoding: n sid_len 4
    key e787a3a491551fb2b4901a3fa15974f3 */

/* cid 26125351da12435615e3be6b16fad35560 sid 0cb227d3 su 65b40b1ab54e05bff55db046 */
/* cid 14de05fc84e41b611dfbe99ed5b1c9d563 sid 6a0f23ad su d73bee2f3a7e72b3ffea52d9 */
/* cid 1306052c3f973db87de6d7904914840ff1 sid ca21402d su 5829465f7418b56ee6ada431 */
/* cid 1d202b5811af3e1dba9ea2950d27879a92 sid b14e1307 su 4902aba8b23a5f24616df3cf */
/* cid 26538b78efc2d418539ad1de13ab73e477 sid a75e0148 su 0040323f1854e75aeb449b9f */

/* LB configuration: cr_bits 0x0 length_self_encoding: y sid_len 5
    key d5a6d7824336fbe0f25d28487cdda57c */

/* cid 10a2794871aadb20ddf274a95249e57fde sid 82d3b0b1a1 su 0935471478c2edb8120e60 */
/* cid 108122fe80a6e546a285c475a3b8613ec9 sid fbcc902c9d su 59c47946882a9a93981c15 */
/* cid 104d227ad9dd0fef4c8cb6eb75887b6ccc sid 2808e22642 su 2a7ef40e2c7e17ae40b3fb */
/* cid 10b3f367d8627b36990a28d67f50b97846 sid 5e018f0197 su 2289cae06a566e5cb6cfa4 */
/* cid 1024412bfe25f4547510204bdda6143814 sid 8a8dd3d036 su 4b12933a135e5eaaebc6fd */
};

picoquic_connection_id_t cid_for_lb_test_init[NB_LB_CONFIG_TEST] = {
    { { 0x08, 0x00, 0x00, 0x00, 0x84, 0x85, 0x86, 0x87 }, 8 },
    { { 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c }, 13},
    { { 0x80, 0x81, 0x82, 0x00, 0x00, 0x00, 0x00, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90 }, 17 },
    /* Test vectors, clear text */
    { { 0 }, 2 }, /* cid 01be sid be su */
    { { 0 , 0, 0xb7}, 3 }, /* cid 0221b7 sid 21 su b7 */
    { { 0, 0, 0xdf, 0xd8}, 4 }, /* cid 03cadfd8 sid ca su dfd8 */
    { { 0, 0, 0x0c, 0x93, 0x28 }, 5 }, /* cid 041e0c9328 sid 1e su 0c9328 */
    { { 0, 0, 0x8f, 0x6d, 0x91, 0x29  }, 6 }, /* cid 050c8f6d9129 sid 0c su 8f6d9129 */
    { { 0, 0, 0}, 3 }, /* cid 02aab0 sid aab0 su */
    { { 0, 0, 0, 0x06}, 4 }, /* cid 3ac4b106 sid c4b1 su 06 */
    { { 0, 0, 0, 0xf4, 0xa0}, 5 }, /* cid 08bd3cf4a0 sid bd3c su f4a0 */
    { { 0, 0, 0, 0x95, 0x02, 0xd6}, 6 }, /* cid 3771d59502d6 sid 71d5 su 9502d6  */
    { { 0, 0, 0, 0xe8, 0xb8, 0x88, 0xf3}, 7 }, /* cid 1d57dee8b888f3 sid 57de su e8b888f3 */
    { { 0, 0, 0, 0}, 4 }, /* cid 0336c976 sid 36c976 su */
    { { 0, 0, 0, 0, 0x06}, 5 }, /* cid 04aa291806 sid aa2918 su 06 */
    { { 0, 0, 0, 0, 0xd8, 0xb6}, 6 }, /* cid 0586897bd8b6 sid 86897b su d8b6 */
    { { 0, 0, 0, 0, 0xae, 0x4d, 0xe0}, 7 }, /* cid 063625bcae4de0 sid 3625bc su ae4de0 */
    { { 0, 0, 0, 0, 0xf3, 0xcb, 0x53, 0x5f}, 8 }, /* cid 07966fb1f3cb535f sid 966fb1 su f3cb535f */
    { { 0, 0, 0, 0, 0}, 5 }, /* cid 185172fab8 sid 5172fab8 su */
    { { 0, 0, 0, 0, 0, 0x97}, 6 }, /* cid 2eb7ff2c9297 sid b7ff2c92 su 97 */
    { { 0, 0, 0, 0, 0, 0xed, 0xbe}, 7 }, /* cid 14f3eb3dd3edbe sid f3eb3dd3 su edbe */
    { { 0, 0, 0, 0, 0, 0x74, 0x4b, 0x74}, 8 }, /* cid 3feb31cece744b74 sid eb31cece su 744b74 */
    { { 0, 0, 0, 0, 0, 0x3c, 0xe2, 0x3b, 0xb5}, 9 }, /* cid 06b9f34c353ce23bb5 sid b9f34c35 su 3ce23bb5 */
    { { 0, 0, 0, 0, 0, 0}, 6 }, /* cid 05bdcd8d0b1d sid bdcd8d0b1d su */
    { { 0, 0, 0, 0, 0, 0, 0x63}, 7 }, /* cid 06aee673725a63 sid aee673725a su 63 */
    { { 0, 0, 0, 0, 0, 0, 0x37, 0xf4}, 8 }, /* cid 07bbf338ddbf37f4 sid bbf338ddbf su 37f4 */
    { { 0, 0, 0, 0, 0, 0, 0x75, 0x68, 0x40}, 9 }, /* cid 08fbbca64c26756840 sid fbbca64c26 su 756840 */
    { { 0, 0, 0, 0, 0, 0, 0x93, 0x89, 0x4e, 0x34}, 10 }, /* cid 09e7737c495b93894e34 sid e7737c495b su 93894e34 */
    /* Test vectors, block cipher */
    /* cid 10564f7c0df399f6d93bdddb1a03886f25 sid 23 su 05231748a80884ed58007847eb9fd0 */
    { { 0, 0, 0x05, 0x23, 0x17, 0x48, 0xa8, 0x08, 0x84, 0xed, 0x58, 0x00, 0x78, 0x47, 0xeb, 0x9f, 0xd0}, 17 },
    /* cid 10d5c03f9dd765d73b3d8610b244f74d02 sid 15 su 76cd6b6f0d3f0b20fc8e633e3a05f3 */
    { { 0, 0, 0x76, 0xcd, 0x6b, 0x6f, 0x0d, 0x3f, 0x0b, 0x20, 0xfc, 0x8e, 0x63, 0x3e, 0x3a, 0x05, 0xf3}, 17 },
    /* cid 108ca55228ab23b92845341344a2f956f2 sid 64 su 65c0ce170a9548717498b537cb8790 */
    { { 0, 0, 0x65, 0xc0, 0xce, 0x17, 0x0a, 0x95, 0x48, 0x71, 0x74, 0x98, 0xb5, 0x37, 0xcb, 0x87, 0x90}, 17 },
    /* cid 10e73f3d034aef2f6f501e3a7693d6270a sid 07 su f9ad10c84cc1e89a2492221d74e707 */
    { { 0, 0, 0xf9, 0xad, 0x10, 0xc8, 0x4c, 0xc1, 0xe8, 0x9a, 0x24, 0x92, 0x22, 0x1d, 0x74, 0xe7, 0x07}, 17 },
    /* cid 101a6ce13d48b14a77ecfd365595ad2582 sid 6c su 76ce4689b0745b956ef71c2608045d */
    { { 0, 0, 0x76, 0xce, 0x46, 0x89, 0xb0, 0x74, 0x5b, 0x95, 0x6e, 0xf7, 0x1c, 0x26, 0x08, 0x04, 0x5d}, 17 },
    /* cid 20aa09bc65ed52b1ccd29feb7ef995d318 sid a52f su 99278b92a86694ff0ecd64bc2f73 */
    { { 0, 0, 0, 0x99, 0x27, 0x8b, 0x92, 0xa8, 0x66, 0x94, 0xff, 0x0e, 0xcd, 0x64, 0xbc, 0x2f, 0x73}, 17 },
    /* cid 30b8dbef657bd78a2f870e93f9485d5211 sid 6c49 su 7381c8657a388b4e9594297afe96 */
    { { 0, 0, 0, 0x73, 0x81, 0xc8, 0x65, 0x7a, 0x38, 0x8b, 0x4e, 0x95, 0x94, 0x29, 0x7a, 0xfe, 0x96}, 17 },
    /* cid 043a8137331eacd2e78383279b202b9a6d sid 4188 su 5ac4b0e0b95f4e7473b49ee2d0dd */
    { { 0, 0, 0, 0x5a, 0xc4, 0xb0, 0xe0, 0xb9, 0x5f, 0x4e, 0x74, 0x73, 0xb4, 0x9e, 0xe2, 0xd0, 0xdd}, 17 },
    /* cid 3ba71ea2bcf0ab95719ab59d3d7fde770d sid 8ccc su 08728807605db25f2ca88be08e0f */
    { { 0, 0, 0, 0x08, 0x72, 0x88, 0x07, 0x60, 0x5d, 0xb2, 0x5f, 0x2c, 0xa8, 0x8b, 0xe0, 0x8e, 0x0f}, 17 },
    /* cid 37ef1956b4ec354f40dc68336a23d42b31 sid c89d su 5a3ccd1471caa0de221ad6c185c0 */
    { { 0, 0, 0, 0x5a, 0x3c, 0xcd, 0x14, 0x71, 0xca, 0xa0, 0xde, 0x22, 0x1a, 0xd6, 0xc1, 0x85, 0xc0}, 17 },
};

picoquic_connection_id_t cid_for_lb_test_ref[NB_LB_CONFIG_TEST] = {
    { { 0x08, 0x00, 0x01, 0x23, 0x84, 0x85, 0x86, 0x87 }, 8 },
    { { 0x8b, 0x7b, 0x37, 0xbe, 0x1c, 0x7c, 0xe2, 0x62, 0x28, 0x66, 0xd9, 0xf1, 0x7a }, 13},
    { { 0x97, 0x42, 0xa4, 0x35, 0x97, 0x2b, 0xfc, 0x60, 0x51, 0x69, 0x1d, 0x28, 0x1a, 0x65, 0x13, 0xcf, 0x4a }, 17 },
    /* Test vectors, clear text */
    { { 0x01, 0xbe}, 2 }, /* cid 01be sid be su */
    { { 0x02, 0x21, 0xb7}, 3 }, /* cid 0221b7 sid 21 su b7 */
    { { 0x03, 0xca, 0xdf, 0xd8}, 4 }, /* cid 03cadfd8 sid ca su dfd8 */
    { { 0x04, 0x1e, 0x0c, 0x93, 0x28 }, 5 }, /* cid 041e0c9328 sid 1e su 0c9328 */
    { { 0x05, 0x0c, 0x8f, 0x6d, 0x91, 0x29  }, 6 }, /* cid 050c8f6d9129 sid 0c su 8f6d9129 */
    { { 0x02, 0xaa, 0xb0}, 3 }, /* cid 02aab0 sid aab0 su */
    { { 0x3a, 0xc4, 0xb1, 0x06}, 4 }, /* cid 3ac4b106 sid c4b1 su 06 */
    { { 0x08, 0xbd, 0x3c, 0xf4, 0xa0}, 5 }, /* cid 08bd3cf4a0 sid bd3c su f4a0 */
    { { 0x37, 0x71, 0xd5, 0x95, 0x02, 0xd6}, 6 }, /* cid 3771d59502d6 sid 71d5 su 9502d6  */
    { { 0x1d, 0x57, 0xde, 0xe8, 0xb8, 0x88, 0xf3}, 7 }, /* cid 1d57dee8b888f3 sid 57de su e8b888f3 */
    { { 0x03, 0x36, 0xc9, 0x76}, 4 }, /* cid 0336c976 sid 36c976 su */
    { { 0x04, 0xaa, 0x29, 0x18, 0x06}, 5 }, /* cid 04aa291806 sid aa2918 su 06 */
    { { 0x05, 0x86, 0x89, 0x7b, 0xd8, 0xb6}, 6 }, /* cid 0586897bd8b6 sid 86897b su d8b6 */
    { { 0x06, 0x36, 0x25, 0xbc, 0xae, 0x4d, 0xe0}, 7 }, /* cid 063625bcae4de0 sid 3625bc su ae4de0 */
    { { 0x07, 0x96, 0x6f, 0xb1, 0xf3, 0xcb, 0x53, 0x5f}, 8 }, /* cid 07966fb1f3cb535f sid 966fb1 su f3cb535f */
    { { 0x18, 0x51, 0x72, 0xfa, 0xb8}, 5 }, /* cid 185172fab8 sid 5172fab8 su */
    { { 0x2e, 0xb7, 0xff, 0x2c, 0x92, 0x97}, 6 }, /* cid 2eb7ff2c9297 sid b7ff2c92 su 97 */
    { { 0x14, 0xf3, 0xeb, 0x3d, 0xd3, 0xed, 0xbe}, 7 }, /* cid 14f3eb3dd3edbe sid f3eb3dd3 su edbe */
    { { 0x3f, 0xeb, 0x31, 0xce, 0xce, 0x74, 0x4b, 0x74}, 8 }, /* cid 3feb31cece744b74 sid eb31cece su 744b74 */
    { { 0x06, 0xb9, 0xf3, 0x4c, 0x35, 0x3c, 0xe2, 0x3b, 0xb5}, 9 }, /* cid 06b9f34c353ce23bb5 sid b9f34c35 su 3ce23bb5 */
    { { 0x05, 0xbd, 0xcd, 0x8d, 0x0b, 0x1d}, 6 }, /* cid 05bdcd8d0b1d sid bdcd8d0b1d su */
    { { 0x06, 0xae, 0xe6, 0x73, 0x72, 0x5a, 0x63}, 7 }, /* cid 06aee673725a63 sid aee673725a su 63 */
    { { 0x07, 0xbb, 0xf3, 0x38, 0xdd, 0xbf, 0x37, 0xf4}, 8 }, /* cid 07bbf338ddbf37f4 sid bbf338ddbf su 37f4 */
    { { 0x08, 0xfb, 0xbc, 0xa6, 0x4c, 0x26, 0x75, 0x68, 0x40}, 9 }, /* cid 08fbbca64c26756840 sid fbbca64c26 su 756840 */
    { { 0x09, 0xe7, 0x73, 0x7c, 0x49, 0x5b, 0x93, 0x89, 0x4e, 0x34}, 10 }, /* cid 09e7737c495b93894e34 sid e7737c495b su 93894e34 */
    /* Test vectors, block cipher */
    /* cid 10564f7c0df399f6d93bdddb1a03886f25 sid 23 su 05231748a80884ed58007847eb9fd0 */
    { { 0x10, 0x56, 0x4f, 0x7c, 0x0d, 0xf3, 0x99, 0xf6, 0xd9, 0x3b, 0xdd, 0xdb, 0x1a, 0x03, 0x88, 0x6f, 0x25}, 17 },
    /* cid 10d5c03f9dd765d73b3d8610b244f74d02 sid 15 su 76cd6b6f0d3f0b20fc8e633e3a05f3 */
    { { 0x10, 0xd5, 0xc0, 0x3f, 0x9d, 0xd7, 0x65, 0xd7, 0x3b, 0x3d, 0x86, 0x10, 0xb2, 0x44, 0xf7, 0x4d, 0x02}, 17 },
    /* cid 108ca55228ab23b92845341344a2f956f2 sid 64 su 65c0ce170a9548717498b537cb8790 */
    { { 0x10, 0x8c, 0xa5, 0x52, 0x28, 0xab, 0x23, 0xb9, 0x28, 0x45, 0x34, 0x13, 0x44, 0xa2, 0xf9, 0x56, 0xf2}, 17 },
    /* cid 10e73f3d034aef2f6f501e3a7693d6270a sid 07 su f9ad10c84cc1e89a2492221d74e707 */
    { { 0x10, 0xe7, 0x3f, 0x3d, 0x03, 0x4a, 0xef, 0x2f, 0x6f, 0x50, 0x1e, 0x3a, 0x76, 0x93, 0xd6, 0x27, 0x0a}, 17 },
    /* cid 101a6ce13d48b14a77ecfd365595ad2582 sid 6c su 76ce4689b0745b956ef71c2608045d */
    { { 0x10, 0x1a, 0x6c, 0xe1, 0x3d, 0x48, 0xb1, 0x4a, 0x77, 0xec, 0xfd, 0x36, 0x55, 0x95, 0xad, 0x25, 0x82}, 17 },
    /* cid 20aa09bc65ed52b1ccd29feb7ef995d318 sid a52f su 99278b92a86694ff0ecd64bc2f73 */
    { { 0x20, 0xaa, 0x09, 0xbc, 0x65, 0xed, 0x52, 0xb1, 0xcc, 0xd2, 0x9f, 0xeb, 0x7e, 0xf9, 0x95, 0xd3, 0x18}, 17 },
    /* cid 30b8dbef657bd78a2f870e93f9485d5211 sid 6c49 su 7381c8657a388b4e9594297afe96 */
    { { 0x30, 0xb8, 0xdb, 0xef, 0x65, 0x7b, 0xd7, 0x8a, 0x2f, 0x87, 0x0e, 0x93, 0xf9, 0x48, 0x5d, 0x52, 0x11}, 17 },
    /* cid 043a8137331eacd2e78383279b202b9a6d sid 4188 su 5ac4b0e0b95f4e7473b49ee2d0dd */
    { { 0x04, 0x3a, 0x81, 0x37, 0x33, 0x1e, 0xac, 0xd2, 0xe7, 0x83, 0x83, 0x27, 0x9b, 0x20, 0x2b, 0x9a, 0x6d}, 17 },
    /* cid 3ba71ea2bcf0ab95719ab59d3d7fde770d sid 8ccc su 08728807605db25f2ca88be08e0f */
    { { 0x3b, 0xa7, 0x1e, 0xa2, 0xbc, 0xf0, 0xab, 0x95, 0x71, 0x9a, 0xb5, 0x9d, 0x3d, 0x7f, 0xde, 0x77, 0x0d}, 17 },
    /* cid 37ef1956b4ec354f40dc68336a23d42b31 sid c89d su 5a3ccd1471caa0de221ad6c185c0 */
    { { 0x37, 0xef, 0x19, 0x56, 0xb4, 0xec, 0x35, 0x4f, 0x40, 0xdc, 0x68, 0x33, 0x6a, 0x23, 0xd4, 0x2b, 0x31}, 17 },
};

int cid_for_lb_test_one(picoquic_quic_t* quic, int test_id, picoquic_load_balancer_config_t* config,
    picoquic_connection_id_t* init_cid, picoquic_connection_id_t* target_cid)
{
    int ret = 0;
    picoquic_connection_id_t result;

    /* Configure the policy */
    ret = picoquic_lb_compat_cid_config(quic, config);

    if (ret != 0) {
        DBG_PRINTF("CID test #%d fails, could not configure the context.\n", test_id);
    }
    else {
        /* Create a CID. */
#if 0
        memset(&result, 0, sizeof(picoquic_connection_id_t));
        for (size_t i = 0; i < quic->local_cnxid_length; i++) {
            result.id[i] = (uint8_t)(0x80 + i);
        }
        result.id_len = quic->local_cnxid_length;
#else
        result = *init_cid;
#endif

        if (quic->cnx_id_callback_fn) {
            quic->cnx_id_callback_fn(quic, picoquic_null_connection_id, picoquic_null_connection_id,
                quic->cnx_id_callback_ctx, &result);
        }

        if (picoquic_compare_connection_id(&result, target_cid) != 0) {
            DBG_PRINTF("CID test #%d fails, result does not match.\n", test_id);
            ret = -1;
        }
        else {
            uint64_t server_id64 = picoquic_lb_compat_cid_verify(quic, quic->cnx_id_callback_ctx, &result);

            if (server_id64 != config->server_id64) {
                DBG_PRINTF("CID test #%d fails, server id decode to %" PRIu64 " instead of %" PRIu64,
                    test_id, server_id64, config->server_id64);
                ret = -1;
            }
        }
    }

    /* Free the configured policy */
    picoquic_lb_compat_cid_config_free(quic);

    return ret;
}


int cid_for_lb_test()
{
    int ret = 0;
    uint64_t simulated_time = 0;
    picoquic_quic_t* quic = picoquic_create(8, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, simulated_time,
        &simulated_time, NULL, NULL, 0);

    if (quic == NULL) {
        DBG_PRINTF("%s", "Could not create the quic context.");
    }
    else {
        for (int i = 0; i < NB_LB_CONFIG_TEST && ret == 0; i++) {
            ret = cid_for_lb_test_one(quic, i, &cid_for_lb_test_config[i], &cid_for_lb_test_init[i], &cid_for_lb_test_ref[i]);
        }

        if (quic != NULL) {
            picoquic_free(quic);
        }
    }
    return ret;
}