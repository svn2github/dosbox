#include "programs.h"
#include "cpu.h"

#include "shell.h"

class APPLICATION;

static APPLICATION *instance;

class APPLICATION : public Program
{
public:
	virtual void Run()
	{
		Bit16u oldcs	= SegValue(cs);
		Bit32u oldeip	= reg_eip;
		Bit16u oldss	= SegValue(ss);
		Bit32u oldesp	= reg_esp;

		// Workaround : Allocate Stack Space
		Bit16u segment;
		Bit16u size = 0x200 / 0x10;
		if (DOS_AllocateMemory(&segment,&size)) {
			SegSet16(ss,segment);
			reg_sp = 0x200;
			ExecuteApp();
			DOS_FreeMemory(segment);
		}
		// set old reg values
		SegSet16(ss,oldss);
		reg_esp = oldesp;
		SegSet16(cs,oldcs);
		reg_eip = oldeip;
	}

private:

	void ExecuteApp()
	{
		DOS_Shell shell;
		shell.Execute("prog", "");
	}
};

void APPLICATION_ProgramStart(Program **program)
{
	*program = new APPLICATION;
}

void APPLICATION_Init(Section* sec)
{
	PROGRAMS_MakeFile("RUNAPP.COM", APPLICATION_ProgramStart);
}
