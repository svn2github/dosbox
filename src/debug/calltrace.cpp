#include "programs.h"
#include "cpu.h"
#include "shell.h"
#include <set>

class CALLTRACE;

static CALLTRACE *instance;

class CALLTRACE : public Program
{
public:

	CALLTRACE() : seg()
	{
		instance = this;
	}

	~CALLTRACE()
	{
		instance = 0;
		Dump();
	}

	virtual void Run()
	{
		// copied from DEBUG::Run
		char filename[128];
		char args[256];

		cmd->FindCommand(1,temp_line);
		safe_strncpy(filename,temp_line.c_str(),128);
		// Read commandline
		Bit16u i	=2;
		args[0]		= 0;
		for (;cmd->FindCommand(i++,temp_line)==true;) {
			strncat(args,temp_line.c_str(),256);
			strncat(args," ",256);
		}
		// Save cpu state....
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
			// Start shell
			DOS_Shell shell;
			shell.Execute(filename,args);
			DOS_FreeMemory(segment);
		}
		// set old reg values
		SegSet16(ss,oldss);
		reg_esp = oldesp;
		SegSet16(cs,oldcs);
		reg_eip = oldeip;
	}

	void Start(Bit16u seg, Bit32u /*off*/)
	{
		this->seg = seg;
	}

	void Trace(Bit16u seg, Bit32u ip)
	{
		if (this->seg != seg)
			return;

		addrs.insert(std::make_pair(seg, ip));
	}

	void Dump()
	{
		FILE *f = fopen("calltrace.log", "wb");
		for (Addresses::iterator it = addrs.begin(), end = addrs.end(); it != end; ++it)
			fprintf(f, "%04x:%08x\n", it->first, it->second);
		fclose(f);
	}

private:

	typedef std::set<std::pair<Bit16u, Bit32u> > Addresses;

	Bit16u seg;
	Addresses addrs;
};

void CALLTRACE_ProgramStart(Program **program)
{
	*program = new CALLTRACE;
}

void CALLTRACE_Init(Section* sec)
{
	PROGRAMS_MakeFile("CALTRACE.COM", CALLTRACE_ProgramStart);
}

void CALLTRACE_StartTrace(Bit16u seg, Bit32u off)
{
	if (!instance)
		return;
	instance->Start(seg, off);
}

void CALLTRACE_Trace()
{
	if (!instance)
		return;

	instance->Trace(SegValue(cs), reg_eip);
}
