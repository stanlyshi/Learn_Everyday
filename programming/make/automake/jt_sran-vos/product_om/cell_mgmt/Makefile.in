######################################################################################
#
#   File: cell_mgmt.mk
#   Time: 2019.03
#   Description: 
#       include file of CELL_MGMT module
#       This is CELL_MGMT module's folder structure.
#       Please "-include $(SRC_ROOT)/product_om/cell_mgmt/cell_mgmt.mk" in makefile
#   Author: Rong Tao
######################################################################################
#
#   Please make "CELL_MGMT_ROOT = $(SRC_ROOT)/product_om/cell_mgmt" in makefile
#   And this file must define:
#       SRC_DIRS
#       INCLUDE_DIRS
#       SOURCES
######################################################################################

CELL_MGMT_ROOT = /mnt/hgfs/shared/jt_sran-vos/product_om/cell_mgmt

INCLUDES  += -I$(CELL_MGMT_ROOT)/include

SRCS += $(CELL_MGMT_ROOT)/src/vcm_cell.c 
SRCS += $(CELL_MGMT_ROOT)/src/vcm_cell_setup.c 
SRCS += $(CELL_MGMT_ROOT)/src/vcm_cell_delete.c 
SRCS += $(CELL_MGMT_ROOT)/src/vcm_cell_stop.c 
SRCS += $(CELL_MGMT_ROOT)/src/vcm_init.c  
SRCS += $(CELL_MGMT_ROOT)/src/vcm_l1l2l3.c  
SRCS += $(CELL_MGMT_ROOT)/src/vcm_module_register.c 
SRCS += $(CELL_MGMT_ROOT)/src/vcm_sm.c  
SRCS += $(CELL_MGMT_ROOT)/src/vcm_tools.c  


