################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../main.cpp \
../MFRC522.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
main.o \
MFRC522.o

OBJS_AS_ARGS +=  \
main.o \
MFRC522.o

C_DEPS +=  \
main.d \
MFRC522.d

C_DEPS_AS_ARGS +=  \
main.d \
MFRC522.d

OUTPUT_FILE_PATH +=RFID.elf

OUTPUT_FILE_PATH_AS_ARGS +=RFID.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler
./main.o: .././main.cpp
	@echo Building file: $<
	@echo Invoking: AVR8/GNU C Compiler : 5.4.0
	$(QUOTE)A:\Atmel\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-g++.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DDEBUG  -I"A:\Atmel\7.0\Packs\atmel\ATmega_DFP\1.2.209\include"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega8a -B "A:\Atmel\7.0\Packs\atmel\ATmega_DFP\1.2.209\gcc\dev\atmega8a" -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./MFRC522.o: .././MFRC522.cpp
	@echo Building file: $<
	@echo Invoking: AVR8/GNU C Compiler : 5.4.0
	$(QUOTE)A:\Atmel\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-g++.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DDEBUG  -I"A:\Atmel\7.0\Packs\atmel\ATmega_DFP\1.2.209\include"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega8a -B "A:\Atmel\7.0\Packs\atmel\ATmega_DFP\1.2.209\gcc\dev\atmega8a" -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	




# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR8/GNU Linker : 5.4.0
	$(QUOTE)A:\Atmel\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="RFID.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega8a -B "A:\Atmel\7.0\Packs\atmel\ATmega_DFP\1.2.209\gcc\dev\atmega8a"  
	@echo Finished building target: $@
	"A:\Atmel\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "RFID.elf" "RFID.hex"
	"A:\Atmel\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "RFID.elf" "RFID.eep" || exit 0
	"A:\Atmel\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "RFID.elf" > "RFID.lss"
	"A:\Atmel\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "RFID.elf" "RFID.srec"
	"A:\Atmel\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "RFID.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "RFID.elf" "RFID.a" "RFID.hex" "RFID.lss" "RFID.eep" "RFID.map" "RFID.srec" "RFID.usersignatures"
	