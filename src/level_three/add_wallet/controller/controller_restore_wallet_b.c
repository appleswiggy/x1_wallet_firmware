/**
 * @file    controller_restore_wallet_b.c
 * @author  Cypherock X1 Team
 * @brief   Restore wallet back controller.
 *          Handles post event (only back/cancel events) operations for restore
 *wallet flow.
 * @copyright Copyright (c) 2022 HODL TECH PTE LTD
 * <br/> You may obtain a copy of license at <a href="https://mitcc.org/"
 *target=_blank>https://mitcc.org/</a>
 *
 ******************************************************************************
 * @attention
 *
 * (c) Copyright 2022 by HODL TECH PTE LTD
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 * "Commons Clause" License Condition v1.0
 *
 * The Software is provided to you by the Licensor under the License,
 * as defined below, subject to the following condition.
 *
 * Without limiting other conditions in the License, the grant of
 * rights under the License will not include, and the License does not
 * grant to you, the right to Sell the Software.
 *
 * For purposes of the foregoing, "Sell" means practicing any or all
 * of the rights granted to you under the License to provide to third
 * parties, for a fee or other consideration (including without
 * limitation fees for hosting or consulting/ support services related
 * to the Software), a product or service whose value derives, entirely
 * or substantially, from the functionality of the Software. Any license
 * notice or attribution required by the License must also include
 * this Commons Clause License Condition notice.
 *
 * Software: All X1Wallet associated files.
 * License: MIT
 * Licensor: HODL TECH PTE LTD
 *
 ******************************************************************************
 */
#include "controller_add_wallet.h"
#include "controller_main.h"
#include "flash_api.h"
#include "tasks.h"

extern Flash_Wallet wallet_for_flash;

static void restore_wallet_enter_mnemonics_controller_b() {
  if (flow_level.level_four > 1) {
    flow_level.level_four--;
  } else {
    flow_level.level_three = RESTORE_WALLET_NUMBER_OF_WORDS_INPUT;
    reset_flow_level_greater_than(LEVEL_THREE);
  }
}

void restore_wallet_controller_b() {
  switch (flow_level.level_three) {
    case RESTORE_WALLET_NAME_CONFIRM: {
      flow_level.level_three = RESTORE_WALLET_NAME_INPUT;
    } break;
    case RESTORE_WALLET_NUMBER_OF_WORDS_INPUT:
      flow_level.level_three = GENERATE_WALLET_NAME_INPUT;
      break;

    case RESTORE_WALLET_ENTER_MNEMONICS:
      restore_wallet_enter_mnemonics_controller_b();
      break;

    case RESTORE_WALLET_VERIFY:
      flow_level.level_three = RESTORE_WALLET_NUMBER_OF_WORDS_INPUT;
      break;

    case RESTORE_WALLET_SKIP_PASSPHRASE: {
      flow_level.level_three =
          RESTORE_WALLET_NUMBER_OF_WORDS_INPUT;    // RESTORE_WALLET_TAP_CARDS;

      WALLET_UNSET_PASSPHRASE(wallet_for_flash.wallet_info);
      WALLET_UNSET_PASSPHRASE(wallet.wallet_info);
    } break;

    case RESTORE_WALLET_SKIP_PASSWORD: {
      if (is_passphrase_enabled())
        flow_level.level_three = RESTORE_WALLET_PASSPHRASE_INSTRUCTIONS_1;
      else
        flow_level.level_three = RESTORE_WALLET_NUMBER_OF_WORDS_INPUT;

      WALLET_UNSET_PIN(wallet_for_flash.wallet_info);
      WALLET_UNSET_PIN(wallet.wallet_info);
    } break;

    case RESTORE_WALLET_PIN_INPUT:
      flow_level.level_three = RESTORE_WALLET_SKIP_PASSWORD;
      break;

    case RESTORE_WALLET_PIN_CONFIRM:
      flow_level.level_three = RESTORE_WALLET_PIN_INPUT;
      break;

    default:
      reset_flow_level_greater_than(LEVEL_ONE);
      flow_level.level_one = LEVEL_TWO_NEW_WALLET;
      counter.level = LEVEL_TWO;
      break;
  }
}