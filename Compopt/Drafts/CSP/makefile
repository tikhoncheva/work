################################################################################
# Makefile
################################################################################

RM := rm -rf

LIBS := -lilocplex -lcp -lcplex -lconcert -lm -lpthread 

INC_cplex = -I/opt/ibm/ILOG/CPLEX_Studio1251/cplex/include
INC_concert = -I/opt/ibm/ILOG/CPLEX_Studio1251/concert/include
INC_cpoptimizer = -I/opt/ibm/ILOG/CPLEX_Studio1251/cpoptimizer/include

LIB_cplex = -L/opt/ibm/ILOG/CPLEX_Studio1251/cplex/lib/x86_sles10_4.1/static_pic
LIB_concert = -L/opt/ibm/ILOG/CPLEX_Studio1251/concert/lib/x86_sles10_4.1/static_pic
LIB_cpoptimizer = -L/opt/ibm/ILOG/CPLEX_Studio1251/cpoptimizer/lib/x86_sles10_4.1/static_pic


Target = OPL_CAP

CPP_SRCS += \
./$(Target).cpp 

OBJS += \
./$(Target).o 

CPP_DEPS += \
./$(Target).d 

all: clean $(Target).o $(Target)

$(Target).o: $(CPP_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DIL_STD $(INC_cplex) $(INC_concert) $(INC_cpoptimizer) -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(Target): $(OBJS) 
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ $(LIB_cplex) $(LIB_concert) $(LIB_cpoptimizer) -o "OPL_CAP" $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(CPP_DEPS)$(EXECUTABLES) OPL_CAP
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
