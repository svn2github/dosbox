#include "programs.h"
#include "cpu.h"

#include "shell.h"

class APPLICATION;

static APPLICATION *instance;

extern EXE_Header app_EXE_header;
extern RealPt app_relocations[];
extern Bitu app_image_size;
extern Bit8u app_image[];


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
		const char * name = "prog.exe";
		const char * args = "";
		char fullname[DOS_PATHLENGTH+4]; //stores results from Which
		const char* p_fullname;
		char line[CMD_MAXLINE];
		line[0]=0;

		/* Check for a full name */
		if (app_image_size == 0)
			p_fullname = DOS_Shell().Which(name);
		else
			p_fullname = name;
		strcpy(fullname,p_fullname);

		/* Run the .exe or .com file from the shell */
		/* Allocate some stack space for tables in physical memory */
		reg_sp-=0x200;
		//Add Parameter block
		DOS_ParamBlock block(SegPhys(ss)+reg_sp);
		block.Clear();
		//Add a filename
		RealPt file_name=RealMakeSeg(ss,reg_sp+0x20);
		MEM_BlockWrite(Real2Phys(file_name),fullname,(Bitu)(strlen(fullname)+1));

		/* Fill the command line */
		CommandTail cmdtail;
		cmdtail.count = 0;
		memset(&cmdtail.buffer,0,126); //Else some part of the string is unitialized (valgrind)
		if (strlen(line)>126) line[126]=0;
		cmdtail.count=(Bit8u)strlen(line);
		memcpy(cmdtail.buffer,line,strlen(line));
		cmdtail.buffer[strlen(line)]=0xd;
		/* Copy command line in stack block too */
		MEM_BlockWrite(SegPhys(ss)+reg_sp+0x100,&cmdtail,128);
		/* Parse FCB (first two parameters) and put them into the current DOS_PSP */
		Bit8u add;
		FCB_Parsename(dos.psp(),0x5C,0x00,cmdtail.buffer,&add);
		FCB_Parsename(dos.psp(),0x6C,0x00,&cmdtail.buffer[add],&add);
		block.exec.fcb1=RealMake(dos.psp(),0x5C);
		block.exec.fcb2=RealMake(dos.psp(),0x6C);
		/* Set the command line in the block and save it */
		block.exec.cmdtail=RealMakeSeg(ss,reg_sp+0x100);
		block.SaveData();
		/* Start up a dos execute interrupt */
		reg_ax=0x4b00;
		//Filename pointer
		SegSet16(ds,SegValue(ss));
		reg_dx=RealOff(file_name);
		//Paramblock
		SegSet16(es,SegValue(ss));
		reg_bx=reg_sp;
		SETFLAGBIT(IF,false);
		if (app_image_size == 0) {
			if (!DOS_Execute(fullname, SegPhys(es)+reg_bx, 0))
				E_Exit("DOS_Execute failed");
		}
		else {
			if (!DOS_Execute(fullname, false, app_EXE_header, app_image_size, app_image, app_relocations, block, 0))
				E_Exit("DOS_Execute failed");
		}
		// RETF
		reg_eip = CPU_Pop16();
		SegSet16(cs,CPU_Pop16());
		cpu.code.big=false;
		DOSBOX_RunMachine(); // not returning until the execution is finished
		/* Restore CS:IP and the stack */
		reg_sp+=0x200;
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
