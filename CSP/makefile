################################################################################
# Makefile
################################################################################

RM := rm -rf

LIBS := -lilocplex -lcp -lcplex -lconcert -lm -lpthread 

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
	g++ -DIL_STD -I/opt/ibm/ILOG/CPLEX_Studio1251/cplex/include -I/opt/ibm/ILOG/CPLEX_Studio1251/concert/include -I/opt/ibm/ILOG/CPLEX_Studio1251/cpoptimizer/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(Target): $(OBJS) 
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L/opt/ibm/ILOG/CPLEX_Studio1251/cplex/lib/x86_sles10_4.1/static_pic -L/opt/ibm/ILOG/CPLEX_Studio1251/concert/lib/x86_sles10_4.1/static_pic -L/opt/ibm/ILOG/CPLEX_Studio1251/cpoptimizer/lib/x86_sles10_4.1/static_pic -o "OPL_CAP" $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(CPP_DEPS)$(EXECUTABLES) OPL_CAP
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
