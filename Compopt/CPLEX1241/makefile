################################################################################
# Makefile
################################################################################

RM := rm -rf

LIBS := -lilocplex -lcp -lcplex -lconcert -lm -lpthread 

INC_cplex = -I/home/tikhoncheva/software/CPLEX_Studio1241/cplex/include
INC_concert = -I/home/tikhoncheva/software/CPLEX_Studio1241/concert/include
INC_cpoptimizer = -I/home/tikhoncheva/software/CPLEX_Studio1241/cpoptimizer/include

LIB_cplex = -L/home/tikhoncheva/software/CPLEX_Studio1241/cplex/lib/x86-64_sles10_4.1/static_pic
LIB_concert = -L/home/tikhoncheva/software/CPLEX_Studio1241/concert/lib/x86-64_sles10_4.1/static_pic
LIB_cpoptimizer = -L/home/tikhoncheva/software/CPLEX_Studio1241/cpoptimizer/lib/x86-64_sles10_4.1/static_pic


Target = CAP_IP
ExTarget = CAP_IP

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
	g++ -DIL_STD $(INC_cplex) $(INC_concert) $(INC_cpoptimizer) -O0 -g -p -pg -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(Target): $(OBJS) 
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ $(LIB_cplex) $(LIB_concert) $(LIB_cpoptimizer) -p -g -pg -o $(ExTarget) $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(CPP_DEPS)$(EXECUTABLES) $(Target) *.lp
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
