#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/sl-vario.pdsv.firmware.master.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/sl-vario.pdsv.firmware.master.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=Interrupts.c LedAnzeige.c SpiConfig.c errorloop.c getSteckplatz.c init.c main.c timing.c Uart.c Eingaegne.c SKR_ZwHd.c GetErrors.c ADC.c Diagnose.c Can.c PortTest.c RamTest.c RomTest.c CCPconfig.c Frequenz.c FrequenzVergleich.c Auswertung.c CRC16.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/Interrupts.o ${OBJECTDIR}/LedAnzeige.o ${OBJECTDIR}/SpiConfig.o ${OBJECTDIR}/errorloop.o ${OBJECTDIR}/getSteckplatz.o ${OBJECTDIR}/init.o ${OBJECTDIR}/main.o ${OBJECTDIR}/timing.o ${OBJECTDIR}/Uart.o ${OBJECTDIR}/Eingaegne.o ${OBJECTDIR}/SKR_ZwHd.o ${OBJECTDIR}/GetErrors.o ${OBJECTDIR}/ADC.o ${OBJECTDIR}/Diagnose.o ${OBJECTDIR}/Can.o ${OBJECTDIR}/PortTest.o ${OBJECTDIR}/RamTest.o ${OBJECTDIR}/RomTest.o ${OBJECTDIR}/CCPconfig.o ${OBJECTDIR}/Frequenz.o ${OBJECTDIR}/FrequenzVergleich.o ${OBJECTDIR}/Auswertung.o ${OBJECTDIR}/CRC16.o
POSSIBLE_DEPFILES=${OBJECTDIR}/Interrupts.o.d ${OBJECTDIR}/LedAnzeige.o.d ${OBJECTDIR}/SpiConfig.o.d ${OBJECTDIR}/errorloop.o.d ${OBJECTDIR}/getSteckplatz.o.d ${OBJECTDIR}/init.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/timing.o.d ${OBJECTDIR}/Uart.o.d ${OBJECTDIR}/Eingaegne.o.d ${OBJECTDIR}/SKR_ZwHd.o.d ${OBJECTDIR}/GetErrors.o.d ${OBJECTDIR}/ADC.o.d ${OBJECTDIR}/Diagnose.o.d ${OBJECTDIR}/Can.o.d ${OBJECTDIR}/PortTest.o.d ${OBJECTDIR}/RamTest.o.d ${OBJECTDIR}/RomTest.o.d ${OBJECTDIR}/CCPconfig.o.d ${OBJECTDIR}/Frequenz.o.d ${OBJECTDIR}/FrequenzVergleich.o.d ${OBJECTDIR}/Auswertung.o.d ${OBJECTDIR}/CRC16.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/Interrupts.o ${OBJECTDIR}/LedAnzeige.o ${OBJECTDIR}/SpiConfig.o ${OBJECTDIR}/errorloop.o ${OBJECTDIR}/getSteckplatz.o ${OBJECTDIR}/init.o ${OBJECTDIR}/main.o ${OBJECTDIR}/timing.o ${OBJECTDIR}/Uart.o ${OBJECTDIR}/Eingaegne.o ${OBJECTDIR}/SKR_ZwHd.o ${OBJECTDIR}/GetErrors.o ${OBJECTDIR}/ADC.o ${OBJECTDIR}/Diagnose.o ${OBJECTDIR}/Can.o ${OBJECTDIR}/PortTest.o ${OBJECTDIR}/RamTest.o ${OBJECTDIR}/RomTest.o ${OBJECTDIR}/CCPconfig.o ${OBJECTDIR}/Frequenz.o ${OBJECTDIR}/FrequenzVergleich.o ${OBJECTDIR}/Auswertung.o ${OBJECTDIR}/CRC16.o

# Source Files
SOURCEFILES=Interrupts.c LedAnzeige.c SpiConfig.c errorloop.c getSteckplatz.c init.c main.c timing.c Uart.c Eingaegne.c SKR_ZwHd.c GetErrors.c ADC.c Diagnose.c Can.c PortTest.c RamTest.c RomTest.c CCPconfig.c Frequenz.c FrequenzVergleich.c Auswertung.c CRC16.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/sl-vario.pdsv.firmware.master.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33CK256MP505
MP_LINKER_FILE_OPTION=,--script=p33CK256MP505.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/Interrupts.o: Interrupts.c  .generated_files/flags/default/3925a88f371a81ea1d7be6f386c9a08162a1cca5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Interrupts.o.d 
	@${RM} ${OBJECTDIR}/Interrupts.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Interrupts.c  -o ${OBJECTDIR}/Interrupts.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Interrupts.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/LedAnzeige.o: LedAnzeige.c  .generated_files/flags/default/c9c696f5d66cf92ba897718899e573036d265d36 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LedAnzeige.o.d 
	@${RM} ${OBJECTDIR}/LedAnzeige.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  LedAnzeige.c  -o ${OBJECTDIR}/LedAnzeige.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/LedAnzeige.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/SpiConfig.o: SpiConfig.c  .generated_files/flags/default/24f3deda42052b30525d487154babe44718ffd7c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SpiConfig.o.d 
	@${RM} ${OBJECTDIR}/SpiConfig.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  SpiConfig.c  -o ${OBJECTDIR}/SpiConfig.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/SpiConfig.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/errorloop.o: errorloop.c  .generated_files/flags/default/319297026b77f5eb39349c0a158c63ce731bbbef .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/errorloop.o.d 
	@${RM} ${OBJECTDIR}/errorloop.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  errorloop.c  -o ${OBJECTDIR}/errorloop.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/errorloop.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/getSteckplatz.o: getSteckplatz.c  .generated_files/flags/default/51d3b6dd5d176c749af7a385f7728c2647f4f8dc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/getSteckplatz.o.d 
	@${RM} ${OBJECTDIR}/getSteckplatz.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  getSteckplatz.c  -o ${OBJECTDIR}/getSteckplatz.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/getSteckplatz.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/init.o: init.c  .generated_files/flags/default/f11c66e24f2f84c03efb042a450fb5a7ce42f40 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/init.o.d 
	@${RM} ${OBJECTDIR}/init.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  init.c  -o ${OBJECTDIR}/init.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/init.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/39f12f909f07e3a42e0a7e2d95ee9525c40a6cf4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/timing.o: timing.c  .generated_files/flags/default/b7a20c0332e9036fb0de0b15a871836bbcd81dd6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timing.o.d 
	@${RM} ${OBJECTDIR}/timing.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  timing.c  -o ${OBJECTDIR}/timing.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/timing.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Uart.o: Uart.c  .generated_files/flags/default/7e7ebc06a823b1240eb8a7c8c6a6a8fcc308c91 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Uart.o.d 
	@${RM} ${OBJECTDIR}/Uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Uart.c  -o ${OBJECTDIR}/Uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Uart.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Eingaegne.o: Eingaegne.c  .generated_files/flags/default/b43ccf7459fcd6db3a550a57f95e10724cafa4f4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Eingaegne.o.d 
	@${RM} ${OBJECTDIR}/Eingaegne.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Eingaegne.c  -o ${OBJECTDIR}/Eingaegne.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Eingaegne.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/SKR_ZwHd.o: SKR_ZwHd.c  .generated_files/flags/default/93401cdf8ad7538bda4e8db71d3923cd864636e3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SKR_ZwHd.o.d 
	@${RM} ${OBJECTDIR}/SKR_ZwHd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  SKR_ZwHd.c  -o ${OBJECTDIR}/SKR_ZwHd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/SKR_ZwHd.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/GetErrors.o: GetErrors.c  .generated_files/flags/default/70b5a3715692ea9c784aaa8173cd35f6e5d27259 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/GetErrors.o.d 
	@${RM} ${OBJECTDIR}/GetErrors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  GetErrors.c  -o ${OBJECTDIR}/GetErrors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/GetErrors.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/ADC.o: ADC.c  .generated_files/flags/default/5984938f8cfdfd0141d7894edbb93838ca67119f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ADC.o.d 
	@${RM} ${OBJECTDIR}/ADC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ADC.c  -o ${OBJECTDIR}/ADC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/ADC.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Diagnose.o: Diagnose.c  .generated_files/flags/default/3fc406d5a4259438cedfc5295da1a662550efa98 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Diagnose.o.d 
	@${RM} ${OBJECTDIR}/Diagnose.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Diagnose.c  -o ${OBJECTDIR}/Diagnose.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Diagnose.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Can.o: Can.c  .generated_files/flags/default/c2e1fe12d54e21261ca3e81922c3ca2d0095b264 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Can.o.d 
	@${RM} ${OBJECTDIR}/Can.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Can.c  -o ${OBJECTDIR}/Can.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Can.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/PortTest.o: PortTest.c  .generated_files/flags/default/aced6e048c7aee3a9007186c8d0bc341ec2ed43e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/PortTest.o.d 
	@${RM} ${OBJECTDIR}/PortTest.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  PortTest.c  -o ${OBJECTDIR}/PortTest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/PortTest.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/RamTest.o: RamTest.c  .generated_files/flags/default/4913b99fbe40ffdd2b77ae5a37debbfea912b56c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RamTest.o.d 
	@${RM} ${OBJECTDIR}/RamTest.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  RamTest.c  -o ${OBJECTDIR}/RamTest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/RamTest.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/RomTest.o: RomTest.c  .generated_files/flags/default/bbfae6192323c4ae1fbb297dffe371f662a46716 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RomTest.o.d 
	@${RM} ${OBJECTDIR}/RomTest.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  RomTest.c  -o ${OBJECTDIR}/RomTest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/RomTest.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/CCPconfig.o: CCPconfig.c  .generated_files/flags/default/75186b0a24bc3ab6c94f4eaeb1712ef85adfd2a0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/CCPconfig.o.d 
	@${RM} ${OBJECTDIR}/CCPconfig.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  CCPconfig.c  -o ${OBJECTDIR}/CCPconfig.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/CCPconfig.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Frequenz.o: Frequenz.c  .generated_files/flags/default/3ccc368445ba689e5233c00093c23fb09a55c012 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Frequenz.o.d 
	@${RM} ${OBJECTDIR}/Frequenz.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Frequenz.c  -o ${OBJECTDIR}/Frequenz.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Frequenz.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/FrequenzVergleich.o: FrequenzVergleich.c  .generated_files/flags/default/4a3558b28f8528074a636188bd69b4cf68e53481 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/FrequenzVergleich.o.d 
	@${RM} ${OBJECTDIR}/FrequenzVergleich.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  FrequenzVergleich.c  -o ${OBJECTDIR}/FrequenzVergleich.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/FrequenzVergleich.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Auswertung.o: Auswertung.c  .generated_files/flags/default/73997ae4c68f125295444e5d97434180db84e963 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Auswertung.o.d 
	@${RM} ${OBJECTDIR}/Auswertung.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Auswertung.c  -o ${OBJECTDIR}/Auswertung.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Auswertung.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/CRC16.o: CRC16.c  .generated_files/flags/default/7047291e7412818d3d4d837a9463726668fddb1a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/CRC16.o.d 
	@${RM} ${OBJECTDIR}/CRC16.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  CRC16.c  -o ${OBJECTDIR}/CRC16.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/CRC16.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
else
${OBJECTDIR}/Interrupts.o: Interrupts.c  .generated_files/flags/default/7768f373899fd074a6e9185dbf25197bf5f973d1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Interrupts.o.d 
	@${RM} ${OBJECTDIR}/Interrupts.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Interrupts.c  -o ${OBJECTDIR}/Interrupts.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Interrupts.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/LedAnzeige.o: LedAnzeige.c  .generated_files/flags/default/b137a079f9a9656ecf0b2d033673ff4746dc452c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LedAnzeige.o.d 
	@${RM} ${OBJECTDIR}/LedAnzeige.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  LedAnzeige.c  -o ${OBJECTDIR}/LedAnzeige.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/LedAnzeige.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/SpiConfig.o: SpiConfig.c  .generated_files/flags/default/67911e8141b3be43176e7e3c497654ea8e5c840 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SpiConfig.o.d 
	@${RM} ${OBJECTDIR}/SpiConfig.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  SpiConfig.c  -o ${OBJECTDIR}/SpiConfig.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/SpiConfig.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/errorloop.o: errorloop.c  .generated_files/flags/default/f4db05ff207289658f5671dee252760497752067 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/errorloop.o.d 
	@${RM} ${OBJECTDIR}/errorloop.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  errorloop.c  -o ${OBJECTDIR}/errorloop.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/errorloop.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/getSteckplatz.o: getSteckplatz.c  .generated_files/flags/default/cd8ad1ee9baae5cba3460ab98f3296523ed0ef3b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/getSteckplatz.o.d 
	@${RM} ${OBJECTDIR}/getSteckplatz.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  getSteckplatz.c  -o ${OBJECTDIR}/getSteckplatz.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/getSteckplatz.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/init.o: init.c  .generated_files/flags/default/85ba1485fbfa48ab17526797db5cc6526dea693d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/init.o.d 
	@${RM} ${OBJECTDIR}/init.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  init.c  -o ${OBJECTDIR}/init.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/init.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/a68c5665e74cc41816088c922201a5f8b6b6a1d1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/timing.o: timing.c  .generated_files/flags/default/7275251fa6d6260495d710022dc99195d2a97daf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timing.o.d 
	@${RM} ${OBJECTDIR}/timing.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  timing.c  -o ${OBJECTDIR}/timing.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/timing.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Uart.o: Uart.c  .generated_files/flags/default/c44820c58093a2423757f228a7343a84c27b756e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Uart.o.d 
	@${RM} ${OBJECTDIR}/Uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Uart.c  -o ${OBJECTDIR}/Uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Uart.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Eingaegne.o: Eingaegne.c  .generated_files/flags/default/f70b4b3cec766106b8c634b3aa1664db7bcaf08f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Eingaegne.o.d 
	@${RM} ${OBJECTDIR}/Eingaegne.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Eingaegne.c  -o ${OBJECTDIR}/Eingaegne.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Eingaegne.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/SKR_ZwHd.o: SKR_ZwHd.c  .generated_files/flags/default/47881a5983b39568b62342f2d741fe14481308ae .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SKR_ZwHd.o.d 
	@${RM} ${OBJECTDIR}/SKR_ZwHd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  SKR_ZwHd.c  -o ${OBJECTDIR}/SKR_ZwHd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/SKR_ZwHd.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/GetErrors.o: GetErrors.c  .generated_files/flags/default/3e6854179c972dfcaecef8fa8bdb98538a4b1e7a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/GetErrors.o.d 
	@${RM} ${OBJECTDIR}/GetErrors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  GetErrors.c  -o ${OBJECTDIR}/GetErrors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/GetErrors.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/ADC.o: ADC.c  .generated_files/flags/default/b16c5d24eb9abe70c7abe00d1ba670c9fe49def2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ADC.o.d 
	@${RM} ${OBJECTDIR}/ADC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ADC.c  -o ${OBJECTDIR}/ADC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/ADC.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Diagnose.o: Diagnose.c  .generated_files/flags/default/c897c3695a2c5acccd949ba1d737da0b8a38308a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Diagnose.o.d 
	@${RM} ${OBJECTDIR}/Diagnose.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Diagnose.c  -o ${OBJECTDIR}/Diagnose.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Diagnose.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Can.o: Can.c  .generated_files/flags/default/94727de6c1c3af41ccc077eb6f3f63724fc1ae2a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Can.o.d 
	@${RM} ${OBJECTDIR}/Can.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Can.c  -o ${OBJECTDIR}/Can.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Can.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/PortTest.o: PortTest.c  .generated_files/flags/default/15888a26bece842a544f38b7a9509893590b7caf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/PortTest.o.d 
	@${RM} ${OBJECTDIR}/PortTest.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  PortTest.c  -o ${OBJECTDIR}/PortTest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/PortTest.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/RamTest.o: RamTest.c  .generated_files/flags/default/5254b829378a8d9d268afd51680b537319232e9d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RamTest.o.d 
	@${RM} ${OBJECTDIR}/RamTest.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  RamTest.c  -o ${OBJECTDIR}/RamTest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/RamTest.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/RomTest.o: RomTest.c  .generated_files/flags/default/5c4e3bba5a1f2c406534cbb12f41bd570b3002cd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RomTest.o.d 
	@${RM} ${OBJECTDIR}/RomTest.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  RomTest.c  -o ${OBJECTDIR}/RomTest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/RomTest.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/CCPconfig.o: CCPconfig.c  .generated_files/flags/default/97952f08fb6b87dd14993ac7e16628f455fe293b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/CCPconfig.o.d 
	@${RM} ${OBJECTDIR}/CCPconfig.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  CCPconfig.c  -o ${OBJECTDIR}/CCPconfig.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/CCPconfig.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Frequenz.o: Frequenz.c  .generated_files/flags/default/99b6d07a36a5bf7e9af15fe58e14a4b93a1fe753 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Frequenz.o.d 
	@${RM} ${OBJECTDIR}/Frequenz.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Frequenz.c  -o ${OBJECTDIR}/Frequenz.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Frequenz.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/FrequenzVergleich.o: FrequenzVergleich.c  .generated_files/flags/default/8ce995eee2d982d284743f507acdebaa07fa02ce .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/FrequenzVergleich.o.d 
	@${RM} ${OBJECTDIR}/FrequenzVergleich.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  FrequenzVergleich.c  -o ${OBJECTDIR}/FrequenzVergleich.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/FrequenzVergleich.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/Auswertung.o: Auswertung.c  .generated_files/flags/default/9544ac0f7309c148a60035add72d9cd0316f2a12 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Auswertung.o.d 
	@${RM} ${OBJECTDIR}/Auswertung.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Auswertung.c  -o ${OBJECTDIR}/Auswertung.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Auswertung.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
${OBJECTDIR}/CRC16.o: CRC16.c  .generated_files/flags/default/4180f38a10b452bddcadf9dd7fcfc81d51faca8c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/CRC16.o.d 
	@${RM} ${OBJECTDIR}/CRC16.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  CRC16.c  -o ${OBJECTDIR}/CRC16.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/CRC16.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -msmall-code -msmall-data -msmall-scalar -mconst-in-code -O0 -falign-arrays -merrata=all -msmart-io=0 -Wall -msfr-warn=off   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/sl-vario.pdsv.firmware.master.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/sl-vario.pdsv.firmware.master.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_ICD4=1  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x1000:0x101B -mreserve=data@0x101C:0x101D -mreserve=data@0x101E:0x101F -mreserve=data@0x1020:0x1021 -mreserve=data@0x1022:0x1023 -mreserve=data@0x1024:0x1027 -mreserve=data@0x1028:0x104F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_ICD4=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--no-handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  
	
else
${DISTDIR}/sl-vario.pdsv.firmware.master.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/sl-vario.pdsv.firmware.master.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--no-handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/sl-vario.pdsv.firmware.master.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
