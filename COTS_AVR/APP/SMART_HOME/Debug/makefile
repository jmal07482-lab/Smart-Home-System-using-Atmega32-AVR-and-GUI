################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include MCAL/Timer1/subdir.mk
-include MCAL/EXTI/subdir.mk
-include MCAL/EEPROM/subdir.mk
-include MCAL/DIO/subdir.mk
-include MCAL/ADC/subdir.mk
-include HAL/Servo/subdir.mk
-include HAL/PB/subdir.mk
-include HAL/LED/subdir.mk
-include HAL/LCD/subdir.mk
-include HAL/KP/subdir.mk
-include APP/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
SMART_HOME.lss \

SIZEDUMMY += \
sizedummy \


# All Target
all: SMART_HOME.elf secondary-outputs

# Tool invocations
SMART_HOME.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,SMART_HOME.map -mmcu=atmega32 -o "SMART_HOME.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

SMART_HOME.lss: SMART_HOME.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S SMART_HOME.elf  >"SMART_HOME.lss"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: SMART_HOME.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 SMART_HOME.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ELFS)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) SMART_HOME.elf
	-@echo ' '

secondary-outputs: $(LSS) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
