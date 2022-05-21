#
# Makefile --
#
# Copyright (c) 2010 Sagemcom Broadband
# 
# The computer program contained herein contains proprietary
# information which is the property of Sagemcom Broadband.
# The program may be used and/or copied only with the written 
# permission of Sagemcom Broadband or in accordance with the
# terms and conditions stipulated in the agreement/contract under
# which the programs have been supplied.
#

include $(CONFIG_PATH)/auto.conf

MODULE_SRCS += ipl_tb_cmd_mb_field.c 
MODULE_SRCS += ipl_tb_cmd_defaults.c
MODULE_SRCS += ipl_tb_cmd_corrupt.c
MODULE_SRCS += ipl_tb_cmd_set_path.c 
MODULE_SRCS += ipl_tb_internal.c
MODULE_SRCS += ipl_tb_cmd_system.c
MODULE_SRCS += ipl_tb_debug_printf.c
MODULE_SRCS += ipl_tb_init.c 
MODULE_SRCS += ipl_tb_term.c 
MODULE_SRCS += ipl_tb_main.c 
MODULE_SRCS += ipl_tb_cmd_read.c 
MODULE_SRCS += ipl_tb_cmd_write.c 
MODULE_SRCS += ipl_tb_cmd_versions.c
MODULE_SRCS += ipl_tb_cmd_quit.c 
MODULE_SRCS += ipl_tb_cmd_reboot.c 
MODULE_SRCS += ipl_tb_mtd_read.c 
MODULE_SRCS += ipl_tb_mtd_erase.c 
MODULE_SRCS += ipl_tb_mtd_write.c
MODULE_SRCS += ipl_tb_nand_test.c
ifdef CONFIG_IPL_BKGD_APP
MODULE_SRCS += ipl_tb_background_test.c
endif
ifdef CONFIG_NAGRA_NASC_3x
MODULE_SRCS += ipl_tb_db3_test.c
endif

MODULE_SRCS += linenoise.c

CFLAGS_MODULE += -Iinclude
CFLAGS_MODULE += -Werror

MODULE_DEPS += IPL_PRT
MODULE_DEPS += INTLDR

ifdef CONFIG_VIACCESS_AUTHENTICATION
MODULE_DEPS += IPL_SGN_VIACCESS
endif

ifdef CONFIG_IPL_SGN_NAGRA
MODULE_DEPS += IPL_SGN_NAGRA
endif


include $(TOOLS_PATH)/rules/modules.mk

