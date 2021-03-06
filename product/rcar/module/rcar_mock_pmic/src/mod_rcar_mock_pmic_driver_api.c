/*
 * Renesas SCP/MCP Software
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <mod_rcar_mock_pmic_private.h>

#include <fwk_id.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static int api_set_enabled(fwk_id_t device_id, bool enable)
{
    struct mod_rcar_mock_pmic_device_ctx *ctx;

    ctx = __mod_rcar_mock_pmic_get_valid_device_ctx(device_id);
    if (ctx == NULL)
        return FWK_E_PARAM;

    ctx->enabled = enable;

    return FWK_SUCCESS;
}

static int api_get_enabled(fwk_id_t device_id, bool *enabled)
{
    struct mod_rcar_mock_pmic_device_ctx *ctx;

    ctx = __mod_rcar_mock_pmic_get_valid_device_ctx(device_id);
    if (ctx == NULL)
        return FWK_E_PARAM;

    *enabled = ctx->enabled;

    return FWK_SUCCESS;
}

static int api_set_voltage(fwk_id_t device_id, uint64_t voltage)
{
    struct mod_rcar_mock_pmic_device_ctx *ctx;
    uint8_t val;

    ctx = __mod_rcar_mock_pmic_get_valid_device_ctx(device_id);
    if (ctx == NULL) {
        return FWK_E_PARAM;
    }

    if (voltage < BD9571MWV_MIN_MV * 1000 || voltage > BD9571MWV_MAX_MV * 1000)
        return -1;

    val = DIV_ROUND(voltage, BD9571MWV_STEP_MV * 1000);
    val &= REG_DATA_DVFS_SetVID_MASK;

#if 0
    int ret;
    /* Not supported because I2C used in kernel */
    ret = rcar_iic_dvfs_send(SLAVE_ADDR_PMIC, REG_ADDR_DVFS_SetVID, val);
    if (ret) {
        return ret;
    }
#endif

    ctx->voltage = voltage;
    return FWK_SUCCESS;
}

static int api_get_voltage(fwk_id_t device_id, uint64_t *voltage)
{
    struct mod_rcar_mock_pmic_device_ctx *ctx;

    ctx = __mod_rcar_mock_pmic_get_valid_device_ctx(device_id);
    if (ctx == NULL)
        return FWK_E_PARAM;

    *voltage = ctx->voltage;

    return FWK_SUCCESS;
}

const struct mod_rcar_pmic_driver_api __mod_rcar_mock_pmic_driver_api = {
    .set_enabled = api_set_enabled,
    .get_enabled = api_get_enabled,
    .set_voltage = api_set_voltage,
    .get_voltage = api_get_voltage,
};
