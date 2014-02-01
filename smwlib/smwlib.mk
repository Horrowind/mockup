##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=smwlib
ConfigurationName      :=Debug
WorkspacePath          := "/home/horrowind/Projects/Mockup"
ProjectPath            := "/home/horrowind/Projects/Mockup/smwlib"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=horrowind
Date                   :=02/01/14
CodeLitePath           :="/home/horrowind/.codelite"
LinkerName             :=g++
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName).so
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="smwlib.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := g++
CC       := gcc
CXXFLAGS :=  -g $(Preprocessors)
CFLAGS   :=  -g $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/pcaddress$(ObjectSuffix) $(IntermediateDirectory)/snesaddress$(ObjectSuffix) $(IntermediateDirectory)/smw_level$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(SharedObjectLinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)
	@$(MakeDirCommand) "/home/horrowind/Projects/Mockup/.build-debug"
	@echo rebuilt > "/home/horrowind/Projects/Mockup/.build-debug/smwlib"

$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/pcaddress$(ObjectSuffix): pcaddress.cpp $(IntermediateDirectory)/pcaddress$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/horrowind/Projects/Mockup/smwlib/pcaddress.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/pcaddress$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/pcaddress$(DependSuffix): pcaddress.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/pcaddress$(ObjectSuffix) -MF$(IntermediateDirectory)/pcaddress$(DependSuffix) -MM "pcaddress.cpp"

$(IntermediateDirectory)/pcaddress$(PreprocessSuffix): pcaddress.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/pcaddress$(PreprocessSuffix) "pcaddress.cpp"

$(IntermediateDirectory)/snesaddress$(ObjectSuffix): snesaddress.cpp $(IntermediateDirectory)/snesaddress$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/horrowind/Projects/Mockup/smwlib/snesaddress.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/snesaddress$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/snesaddress$(DependSuffix): snesaddress.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/snesaddress$(ObjectSuffix) -MF$(IntermediateDirectory)/snesaddress$(DependSuffix) -MM "snesaddress.cpp"

$(IntermediateDirectory)/snesaddress$(PreprocessSuffix): snesaddress.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/snesaddress$(PreprocessSuffix) "snesaddress.cpp"

$(IntermediateDirectory)/smw_level$(ObjectSuffix): smw_level.cpp $(IntermediateDirectory)/smw_level$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/horrowind/Projects/Mockup/smwlib/smw_level.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/smw_level$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/smw_level$(DependSuffix): smw_level.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/smw_level$(ObjectSuffix) -MF$(IntermediateDirectory)/smw_level$(DependSuffix) -MM "smw_level.cpp"

$(IntermediateDirectory)/smw_level$(PreprocessSuffix): smw_level.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/smw_level$(PreprocessSuffix) "smw_level.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/pcaddress$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/pcaddress$(DependSuffix)
	$(RM) $(IntermediateDirectory)/pcaddress$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/snesaddress$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/snesaddress$(DependSuffix)
	$(RM) $(IntermediateDirectory)/snesaddress$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/smw_level$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/smw_level$(DependSuffix)
	$(RM) $(IntermediateDirectory)/smw_level$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) "../.build-debug/smwlib"


