################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"D:/AAA__Workspace/Software/TI_CCS/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"D:/AAA__Workspace/CCS/proj0/empty_LP_MSPM0G3507_nortos_ticlang/BSP/MPU6050" -I"D:/AAA__Workspace/CCS/proj0/empty_LP_MSPM0G3507_nortos_ticlang/BSP/MPU6050/DMP" -I"D:/AAA__Workspace/CCS/proj0/empty_LP_MSPM0G3507_nortos_ticlang/CONTROL" -I"D:/AAA__Workspace/CCS/proj0/empty_LP_MSPM0G3507_nortos_ticlang/HARDWARE" -I"D:/AAA__Workspace/CCS/proj0/empty_LP_MSPM0G3507_nortos_ticlang" -I"D:/AAA__Workspace/CCS/proj0/empty_LP_MSPM0G3507_nortos_ticlang/Debug" -I"D:/AAA__Workspace/Software/TI_CCS/mspm0_sdk_2_10_00_04/source/third_party/CMSIS/Core/Include" -I"D:/AAA__Workspace/Software/TI_CCS/mspm0_sdk_2_10_00_04/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1513806572: ../empty.syscfg
	@echo 'SysConfig - building file: "$<"'
	"C:/TI/sysconfig_1.26.2/sysconfig_cli.bat" -s "D:/AAA__Workspace/Software/TI_CCS/mspm0_sdk_2_10_00_04/.metadata/product.json" --script "D:/AAA__Workspace/CCS/proj0/empty_LP_MSPM0G3507_nortos_ticlang/empty.syscfg" -o "." --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

device_linker.cmd: build-1513806572 ../empty.syscfg
device.opt: build-1513806572
device.cmd.genlibs: build-1513806572
ti_msp_dl_config.c: build-1513806572
ti_msp_dl_config.h: build-1513806572
Event.dot: build-1513806572

%.o: ./%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"D:/AAA__Workspace/Software/TI_CCS/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"D:/AAA__Workspace/CCS/proj0/empty_LP_MSPM0G3507_nortos_ticlang/BSP/MPU6050" -I"D:/AAA__Workspace/CCS/proj0/empty_LP_MSPM0G3507_nortos_ticlang/BSP/MPU6050/DMP" -I"D:/AAA__Workspace/CCS/proj0/empty_LP_MSPM0G3507_nortos_ticlang/CONTROL" -I"D:/AAA__Workspace/CCS/proj0/empty_LP_MSPM0G3507_nortos_ticlang/HARDWARE" -I"D:/AAA__Workspace/CCS/proj0/empty_LP_MSPM0G3507_nortos_ticlang" -I"D:/AAA__Workspace/CCS/proj0/empty_LP_MSPM0G3507_nortos_ticlang/Debug" -I"D:/AAA__Workspace/Software/TI_CCS/mspm0_sdk_2_10_00_04/source/third_party/CMSIS/Core/Include" -I"D:/AAA__Workspace/Software/TI_CCS/mspm0_sdk_2_10_00_04/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_mspm0g350x_ticlang.o: D:/AAA__Workspace/Software/TI_CCS/mspm0_sdk_2_10_00_04/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"D:/AAA__Workspace/Software/TI_CCS/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"D:/AAA__Workspace/CCS/proj0/empty_LP_MSPM0G3507_nortos_ticlang/BSP/MPU6050" -I"D:/AAA__Workspace/CCS/proj0/empty_LP_MSPM0G3507_nortos_ticlang/BSP/MPU6050/DMP" -I"D:/AAA__Workspace/CCS/proj0/empty_LP_MSPM0G3507_nortos_ticlang/CONTROL" -I"D:/AAA__Workspace/CCS/proj0/empty_LP_MSPM0G3507_nortos_ticlang/HARDWARE" -I"D:/AAA__Workspace/CCS/proj0/empty_LP_MSPM0G3507_nortos_ticlang" -I"D:/AAA__Workspace/CCS/proj0/empty_LP_MSPM0G3507_nortos_ticlang/Debug" -I"D:/AAA__Workspace/Software/TI_CCS/mspm0_sdk_2_10_00_04/source/third_party/CMSIS/Core/Include" -I"D:/AAA__Workspace/Software/TI_CCS/mspm0_sdk_2_10_00_04/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


