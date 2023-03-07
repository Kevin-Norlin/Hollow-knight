##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=Hollow-knight-src
ConfigurationName      :=Debug
WorkspaceConfiguration :=Debug
WorkspacePath          :=C:/Users/kevin/mop/Hollow-knight-MD407/Workspace
ProjectPath            :=C:/Users/kevin/mop/Hollow-knight-MD407/Workspace/Hollow-knight-src
IntermediateDirectory  :=$(ConfigurationName)
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=kevin
Date                   :=07/03/2023
CodeLitePath           :=C:/cseapp/CodeLite
LinkerName             :=$(CodeLiteDir)/tools/gcc-arm/bin/arm-none-eabi-g++.exe
SharedObjectLinkerName :=$(CodeLiteDir)/tools/gcc-arm/arm-none-eabi-g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputDirectory        :=$(IntermediateDirectory)
OutputFile             :=$(IntermediateDirectory)/$(ProjectName).exe
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="Hollow-knight-src.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=
LinkOptions            :=  -T$(ProjectPath)/md407-ram.x -L$(ARM_V6LIB) -L$(ARM_GCC_V6LIB) -nostartfiles
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)gcc $(LibrarySwitch)c_nano 
ArLibs                 :=  "gcc" "c_nano" 
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overridden using an environment variable
##
AR       := $(CodeLiteDir)/tools/gcc-arm/bin/arm-none-eabi-ar.exe rcu
CXX      := $(CodeLiteDir)/tools/gcc-arm/bin/arm-none-eabi-g++.exe
CC       := $(CodeLiteDir)/tools/gcc-arm/bin/arm-none-eabi-gcc.exe
CXXFLAGS :=  -g -O0 -W $(Preprocessors)
CFLAGS   :=  -g -O0 -w -mthumb -march=armv6-m  -mfloat-abi=soft -std=c99 $(Preprocessors)
ASFLAGS  := 
AS       := $(CodeLiteDir)/tools/gcc-arm/bin/arm-none-eabi-as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\cseapp\CodeLite
ARM_V6LIB:=$(CodeLiteDir)/tools/gcc-arm/arm-none-eabi/lib/thumb/v6-m/nofp
ARM_GCC_V6LIB:=$(CodeLiteDir)/tools/gcc-arm/lib/gcc/arm-none-eabi/9.2.1/thumb/v6-m/nofp
ARM_M4SFPLIB:=$(CodeLiteDir)/tools/gcc-arm/arm-none-eabi/lib/thumb/v7e-m+fp/softfp
ARM_GCC_M4SFPLIB:=$(CodeLiteDir)/tools/gcc-arm/lib/gcc/arm-none-eabi/9.2.1/thumb/v7e-m+fp/softfp
ARM_M4HFPLIB:=$(CodeLiteDir)/tools/gcc-arm/arm-none-eabi/lib/thumb/v7e-m+fp/hard
ARM_GCC_M4HFPLIB:=$(CodeLiteDir)/tools/gcc-arm/lib/gcc/arm-none-eabi/9.2.1/thumb/v7e-m+fp/hard
Objects0=$(IntermediateDirectory)/logic.c$(ObjectSuffix) $(IntermediateDirectory)/objects.c$(ObjectSuffix) $(IntermediateDirectory)/startup.c$(ObjectSuffix) $(IntermediateDirectory)/keyb.c$(ObjectSuffix) $(IntermediateDirectory)/graphics.c$(ObjectSuffix) $(IntermediateDirectory)/ascii.c$(ObjectSuffix) $(IntermediateDirectory)/delay.c$(ObjectSuffix) $(IntermediateDirectory)/timer6.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

PostBuild:
	@echo Executing Post Build commands ...
	C:\cseapp\CodeLite/tools/gcc-arm/bin/arm-none-eabi-objcopy -S -O srec  Debug/Hollow-knight-src Debug/Hollow-knight-src.s19
	C:\cseapp\CodeLite/tools/gcc-arm/bin/arm-none-eabi-objdump -D -S Debug/Hollow-knight-src > Debug/Hollow-knight-src.dass
	@echo Done

MakeIntermediateDirs:
	@$(MakeDirCommand) "$(ConfigurationName)"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "$(ConfigurationName)"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/logic.c$(ObjectSuffix): logic.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/logic.c$(ObjectSuffix) -MF$(IntermediateDirectory)/logic.c$(DependSuffix) -MM logic.c
	$(CC) $(SourceSwitch) "C:/Users/kevin/mop/Hollow-knight-MD407/Workspace/Hollow-knight-src/logic.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/logic.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/logic.c$(PreprocessSuffix): logic.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/logic.c$(PreprocessSuffix) logic.c

$(IntermediateDirectory)/objects.c$(ObjectSuffix): objects.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/objects.c$(ObjectSuffix) -MF$(IntermediateDirectory)/objects.c$(DependSuffix) -MM objects.c
	$(CC) $(SourceSwitch) "C:/Users/kevin/mop/Hollow-knight-MD407/Workspace/Hollow-knight-src/objects.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/objects.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/objects.c$(PreprocessSuffix): objects.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/objects.c$(PreprocessSuffix) objects.c

$(IntermediateDirectory)/startup.c$(ObjectSuffix): startup.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/startup.c$(ObjectSuffix) -MF$(IntermediateDirectory)/startup.c$(DependSuffix) -MM startup.c
	$(CC) $(SourceSwitch) "C:/Users/kevin/mop/Hollow-knight-MD407/Workspace/Hollow-knight-src/startup.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/startup.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/startup.c$(PreprocessSuffix): startup.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/startup.c$(PreprocessSuffix) startup.c

$(IntermediateDirectory)/keyb.c$(ObjectSuffix): keyb.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/keyb.c$(ObjectSuffix) -MF$(IntermediateDirectory)/keyb.c$(DependSuffix) -MM keyb.c
	$(CC) $(SourceSwitch) "C:/Users/kevin/mop/Hollow-knight-MD407/Workspace/Hollow-knight-src/keyb.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/keyb.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/keyb.c$(PreprocessSuffix): keyb.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/keyb.c$(PreprocessSuffix) keyb.c

$(IntermediateDirectory)/graphics.c$(ObjectSuffix): graphics.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/graphics.c$(ObjectSuffix) -MF$(IntermediateDirectory)/graphics.c$(DependSuffix) -MM graphics.c
	$(CC) $(SourceSwitch) "C:/Users/kevin/mop/Hollow-knight-MD407/Workspace/Hollow-knight-src/graphics.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/graphics.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/graphics.c$(PreprocessSuffix): graphics.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/graphics.c$(PreprocessSuffix) graphics.c

$(IntermediateDirectory)/ascii.c$(ObjectSuffix): ascii.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ascii.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ascii.c$(DependSuffix) -MM ascii.c
	$(CC) $(SourceSwitch) "C:/Users/kevin/mop/Hollow-knight-MD407/Workspace/Hollow-knight-src/ascii.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ascii.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ascii.c$(PreprocessSuffix): ascii.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ascii.c$(PreprocessSuffix) ascii.c

$(IntermediateDirectory)/delay.c$(ObjectSuffix): delay.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/delay.c$(ObjectSuffix) -MF$(IntermediateDirectory)/delay.c$(DependSuffix) -MM delay.c
	$(CC) $(SourceSwitch) "C:/Users/kevin/mop/Hollow-knight-MD407/Workspace/Hollow-knight-src/delay.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/delay.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/delay.c$(PreprocessSuffix): delay.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/delay.c$(PreprocessSuffix) delay.c

$(IntermediateDirectory)/timer6.c$(ObjectSuffix): timer6.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/timer6.c$(ObjectSuffix) -MF$(IntermediateDirectory)/timer6.c$(DependSuffix) -MM timer6.c
	$(CC) $(SourceSwitch) "C:/Users/kevin/mop/Hollow-knight-MD407/Workspace/Hollow-knight-src/timer6.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/timer6.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/timer6.c$(PreprocessSuffix): timer6.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/timer6.c$(PreprocessSuffix) timer6.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(ConfigurationName)/


