################################################################################
# Makefile
################################################################################

RM := rm -rf

LIBS := -lilocplex -lcp -lcplex -lconcert -lm -lpthread 

INC_cplex = -I/home/kitty/Software/CPLEX_Studio126/cplex/include
INC_concert = -I/home/kitty/Software/CPLEX_Studio126/concert/include
INC_cpoptimizer = -I/home/kitty/Software/CPLEX_Studio126/cpoptimizer/include

LIB_cplex = -L/home/kitty/Software/CPLEX_Studio126/cplex/lib/x86-64_linux/static_pic
LIB_concert = -L/home/kitty/Software/CPLEX_Studio126/concert/lib/x86-64_linux/static_pic
LIB_cpoptimizer = -L/home/kitty/Software/CPLEX_Studio126/cpoptimizer/lib/x86-64_linux/static_pic


Target = CAPmain
buildingTarget = CAPbranchAndCut

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
	@echo 'Building target: $(buildingTarget)'
	
	@echo 'Invoking: GCC C++ Linker'
	g++ $(LIB_cplex) $(LIB_concert) $(LIB_cpoptimizer) -o $(buildingTarget) $(OBJS) $(LIBS)
	@echo 'Finished building target: $(buildingTarget)'
	
	@echo ' '

# Other Targets
clean:
	# -$(RM) *.lp $(OBJS)$(CPP_DEPS)$(EXECUTABLES)$(buldingTarget)
	-$(RM) *.o *.d *.lp $(buildingTarget) 
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
