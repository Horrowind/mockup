#include "asar.h"
#include "libstr.h"
#include "libsmw.h"
#include "scapegoat.hpp"
#include "autoarray.h"
//#include <stdio.h>

int arch=arch_65816;

extern int pass;

int snespos;
int realsnespos;
int startpos;
int realstartpos;

bool emulatexkas;

extern int optimizeforbank;

template<typename t> void error(int neededpass, const char * e_);
void warn(const char * e);

static bool movinglabelspossible=false;

#define error(p, e) error<errblock>(p, e)

int bytes;

static enum {
	ratsmeta_ban,
	ratsmeta_allow,
	ratsmeta_used,
} ratsmetastate=ratsmeta_ban;

int snestopc_pick(int addr)
{
	return snestopc(addr);
}

inline void verifysnespos()
{
	if (snespos<0 || realsnespos<0)
	{
		warn0("Missing org or freespace command");
		snespos=0x008000;
		realsnespos=0x008000;
		startpos=0x008000;
		realstartpos=0x008000;
	}
}

inline void step(int num)
{
	snespos+=num;
	realsnespos+=num;
	bytes+=num;
}

inline void write1_65816(unsigned int num)
{
	verifysnespos();
	if (pass==2)
	{
		int pcpos=snestopc(realsnespos&0xFFFFFF);
		if (pcpos<0)
		{
			movinglabelspossible=true;
			error(2, S"SNES address $"+hex6(realsnespos)+" doesn't map to ROM");
		}
		romdata[pcpos]=num;
		if (pcpos>=romlen) romlen=pcpos+1;
	}
	step(1);
	ratsmetastate=ratsmeta_ban;
}

//inline void write1_generic16(unsigned int num)
//{
//	if (pass==2)
//	{
//		if (realsnespos&0xFFFF0000) error(2, "Address out of bounds");
//		int pcpos=realsnespos&0xFFFF;
//		romdata[pcpos]=num;
//		if (pcpos>=romlen) romlen=pcpos+1;
//	}
//	step(1);
//}

//inline void write1_generic24(unsigned int num)
//{
//	if (pass==2)
//	{
//		if (realsnespos&0xFF000000) error(2, "Address out of bounds");
//		int pcpos=realsnespos&0xFFFFFF;
//		romdata[pcpos]=num;
//		if (pcpos>=romlen) romlen=pcpos+1;
//	}
//	step(1);
//}

void write1_pick(unsigned int num)
{
	//verifysnespos();
	write1_65816(num);
}

void asinit_65816();
bool asblock_65816(char** word, int numwords);
void asend_65816();
void asinit_spc700();
bool asblock_spc700(char** word, int numwords);
void asend_spc700();
void asinit_superfx();
bool asblock_superfx(char** word, int numwords);
void asend_superfx();

bool asblock_pick(char** word, int numwords)
{
	if (arch==arch_65816) return asblock_65816(word, numwords);
	if (arch==arch_spc700) return asblock_spc700(word, numwords);
	if (arch==arch_spc700_inline) return asblock_spc700(word, numwords);
	if (arch==arch_superfx) return asblock_superfx(word, numwords);
	return true;
}

#define write1 write1_pick
#define snestopc snestopc_pick

const char * safedequote(char * str)
{
	const char * tmp=dequote(str);
	if (!tmp) error(0, "Garbage near quoted string");
	return tmp;
}
#define dequote safedequote

extern char romtitle[30];
extern bool stdlib;

extern bool foundlabel;
extern bool forwardlabel;
int getlen(const char * str);
int getnum(const char * str);

void assemblefile(const char * filename, bool toplevel);
extern const char * thisfilename;
extern int thisline;

const char * createuserfunc(const char * name, const char * arguments, const char * content);

bool commandlineflag(const char * cmd, const char * val);

void write2(unsigned int num)
{
	write1(num);
	write1(num/256);
	//if (pass==2)
	//{
	//	int pcpos=snestopc(realsnespos&0xFFFFFF);
	//	if (pcpos<0) error(S"SNES address "+hex6(realsnespos)+" doesn't map to ROM");
	//	romdata_r[pcpos+0]=num    ;
	//	romdata_r[pcpos+1]=num>> 8;
	//	if (pcpos+2>romlen) romlen=pcpos+2;
	//}
	//step(2);
}

void write3(unsigned int num)
{
	write1(num);
	write1(num/256);
	write1(num/65536);
	//if (pass==2)
	//{
	//	int pcpos=snestopc(realsnespos&0xFFFFFF);
	//	if (pcpos<0) error(S"SNES address "+hex6(realsnespos)+" doesn't map to ROM");
	//	romdata_r[pcpos+0]=num    ;
	//	romdata_r[pcpos+1]=num>> 8;
	//	romdata_r[pcpos+2]=num>>16;
	//	if (pcpos+3>romlen) romlen=pcpos+3;
	//}
	//step(3);
}

void write4(unsigned int num)
{
	write1(num);
	write1(num/256);
	write1(num/65536);
	write1(num/16777216);
	//if (pass==2)
	//{
	//	int pcpos=snestopc(realsnespos&0xFFFFFF);
	//	if (pcpos<0) error(S"SNES address "+hex6(realsnespos)+" doesn't map to ROM");
	//	romdata_r[pcpos+0]=num    ;
	//	romdata_r[pcpos+1]=num>> 8;
	//	romdata_r[pcpos+2]=num>>16;
	//	romdata_r[pcpos+3]=num>>24;
	//	if (pcpos+4>romlen) romlen=pcpos+4;
	//}
	//step(4);
}

//these are NOT used by the math parser - see math.cpp for that
int read1(int snespos)
{
	int addr=snestopc(snespos);
	if (addr<0 || addr+1>romlen_r) return -1;
	return
			 romdata_r[addr  ]     ;
}

int read2(int snespos)
{
	//return
	//		read1(snespos  )    |
	//		read1(snespos+1)<< 8;
	int addr=snestopc(snespos);
	if (addr<0 || addr+2>romlen_r) return -1;
	return
			 romdata_r[addr  ]     |
			(romdata_r[addr+1]<< 8);
}

int read3(int snespos)
{
	//return
	//		read1(snespos  )    |
	//		read1(snespos+1)<< 8|
	//		read1(snespos+2)<<16;
	int addr=snestopc(snespos);
	if (addr<0 || addr+3>romlen_r) return -1;
	return
			 romdata_r[addr  ]     |
			(romdata_r[addr+1]<< 8)|
			(romdata_r[addr+2]<<16);
}

int getlenfromchar(char c)
{
	c=tolower(c);
	if (c=='b') return 1;
	if (c=='w') return 2;
	if (c=='l') return 3;
	if (c=='d') return 4;
	error(0, "Invalid size modifier");
	return -1;
}

lightweight_map<string, unsigned int> labels;
autoarray<int> poslabels;
autoarray<int> neglabels;

autoarray<string> sublabels;
string ns;

//bool fastrom=false;

void startmacro(const char * line);
void tomacro(const char * line);
void endmacro(bool insert);
void callmacro(const char * data);

extern int reallycalledmacros;
extern int calledmacros;
extern int macrorecursion;
extern int incsrcdepth;

extern int repeatnext;

string dir(char const *name);

bool confirmname(const char * name)
{
	if (!name[0]) return false;
	if (isdigit(name[0])) return false;
	for (int i=0;name[i];i++)
	{
		if (!isalnum(name[i]) && name[i]!='_') return false;
	}
	return true;
}

string labelname(const char ** rawname, bool define=false)
{
#define rawname (*rawname)
	string name;
	int i=-1;
	if (isdigit(*rawname)) error(0, "Broken label name");
	if (*rawname==':')
	{
		rawname++;
		name=":";
	}
	else if (*rawname=='?')
	{
		if (!macrorecursion) error(0, "Macro label outside of a macro");
		name=S":macro"+dec(calledmacros)+"_";
		rawname++;
	}
	else
	{
		for (i=0;(*rawname=='.');i++) rawname++;
		if (!isualnum(*rawname)) error(0, "Invalid label name.");
		if (emulatexkas && i>1) warn0("Convert the patch to native Asar format instead of making an Asar-only xkas patch.");
		if (i)
		{
			if (!sublabels[i-1]) error(0, "This label has no parent.");
			name+=S sublabels[i-1]+"_";
		}
	}
	if (!isualnum(*rawname)) error(0, "Invalid label name.");
	while (isualnum(*rawname))
	{
		name+=*(rawname++);
	}
	if (define && i>=0)
	{
		sublabels.reset(i);
		sublabels[i]=name;
	}
	return name;
#undef rawname
}

string labelname(char ** rawname, bool define=false)
{
	return labelname((const char**)rawname, define);
}

inline bool labelvalcore(const char ** rawname, unsigned int * rval, bool define, bool shouldthrow)
{
	string name=labelname(rawname, define);
	unsigned int rval_=0;
	if (ns && labels.find(S ns+"_"+name, rval_)) {}
	else if (labels.find(name, rval_)) {}
	else
	{
		if (shouldthrow && pass) error(1, S"Label "+name+" not found");
		if (rval) *rval=-1;
		return false;
	}
	if (rval)
	{
		*rval=rval_;
		//if (fastrom && (rval_&0x700000)!=0x700000) *rval|=0x800000;
	}
	return true;
}

unsigned int labelval(const char ** rawname, bool define)
{
	unsigned int rval;
	labelvalcore(rawname, &rval, define, true);
	return rval;
}

unsigned int labelval(char ** rawname, bool define)
{
	unsigned int rval;
	labelvalcore((const char**)rawname, &rval, define, true);
	return rval;
}

unsigned int labelval(string name, bool define)
{
	const char * rawname=name;
	unsigned int rval;
	labelvalcore(&rawname, &rval, define, true);
	return rval;
}

bool labelval(const char ** rawname, unsigned int * rval, bool define)
{
	return labelvalcore(rawname, rval, define, false);
}

bool labelval(char ** rawname, unsigned int * rval, bool define)
{
	return labelvalcore((const char**)rawname, rval, define, false);
}

bool labelval(string name, unsigned int * rval, bool define)
{
	const char * str=name;
	return labelvalcore(&str, rval, define, false);
}

void setlabel(string name, int loc=-1)
{
	if (loc==-1)
	{
		verifysnespos();
		loc=snespos;
	}
	unsigned int labelpos;
	if (pass==0)
	{
		if (labels.find(name, labelpos))
		{
			movinglabelspossible=true;
			error(0, S"Label \""+name+"\" redefined");
		}
		labels.insert(name, loc);
	}
	else if (pass==1)
	{
		labels.insert(name, loc);
	}
	else if (pass==2)
	{
		//all label locations are known at this point, add a sanity check
		if (!labels.find(name, labelpos)) error(2, "Internal error: A label was created on the third pass. Send this patch to Alcaro so he can debug it.");
		if ((int)labelpos!=loc && !movinglabelspossible) error(2, "Internal error: A label is moving around. Send this patch to Alcaro so he can debug it.");
	}
}

unsigned int table[256];

int freespacepos[256];
int freespacelen[256];
int freespaceidnext;
int freespaceid;
int freespacestart;
int freespaceextra;

bool freespaceleak[256];
string freespacefile[256];
int freespaceline[256];

int freespaceorgpos[256];
int freespaceorglen[256];
bool freespacestatic[256];

void cleartable()
{
	for (int i=0;i<256;i++) table[i]=i;
}

struct pushable {
int arch;
int snespos;
int snesstart;
int snesposreal;
int snesstartreal;
int freeid;
int freeex;
int freest;
int arch1;
int arch2;
int arch3;
int arch4;
};
autoarray<pushable> pushpc;
int pushpcnum;

unsigned char fillbyte[12];
unsigned char padbyte[12];

bool sandbox=false;

int getfreespaceid()
{
	if (freespaceidnext>125) fatalerror("A patch may not contain more than 125 freespaces.");
	return freespaceidnext++;
}

void checkbankcross()
{
	if (snespos<0 && realsnespos<0 && startpos<0 && realstartpos<0) return;
//puts(S hex(snespos)+"^"+hex(startpos)+"&0xFFFF0000 ("+hex((snespos^startpos)&0xFFFF0000)+")");
//puts(S hex(realsnespos)+"^"+hex(realstartpos)+"&0xFFFF0000 ("+hex((realsnespos^realstartpos)&0xFFFF0000)+")");
	if ((((    snespos^    startpos)&0x7FFF0000) && (((    snespos-1)^    startpos)&0x7FFF0000)) ||
			(((realsnespos^realstartpos)&0x7FFF0000) && (((realsnespos-1)^realstartpos)&0x7FFF0000)))
				fatalerror("A bank border was crossed somewhere prior to this point.");
}

void freespaceend()
{
	checkbankcross();
	if ((snespos&0x7F000000) && (snespos&0x80000000)==0)
	{
		freespacelen[freespaceid]=snespos-freespacestart+freespaceextra;
		snespos=0xFFFFFFFF;
	}
	freespaceextra=0;
}

int savedoptions[16];

int numopcodes;

extern bool math_pri;
extern bool math_round;

bool warnxkas;

#include "scapegoat.hpp"
extern lightweight_map<string, string> defines;

void initstuff()
{
	//int numsavedoptions=0;
	if (pass==0)
	{
		for (int i=0;i<256;i++)
		{
			freespaceleak[i]=true;
			freespaceorgpos[i]=-2;
			freespaceorglen[i]=-1;
		}
	}
	arch=arch_65816;
	mapper=lorom;
	reallycalledmacros=0;
	calledmacros=0;
	macrorecursion=0;
	repeatnext=1;
	defines.clear();
	ns="";
	sublabels.reset();
	poslabels.reset();
	neglabels.reset();
	cleartable();
	pushpc.reset();
	pushpcnum=0;
	bytes=0;
	memset(fillbyte, 0, sizeof(fillbyte));
	memset(padbyte, 0, sizeof(padbyte));
	snespos=0xFFFFFFFF;
	realsnespos=0xFFFFFFFF;
	startpos=0xFFFFFFFF;
	realstartpos=0xFFFFFFFF;
	//fastrom=false;
	freespaceidnext=1;
	freespaceid=1;
	freespaceextra=0;
	numopcodes=0;
	
	math_pri=false;
	math_round=true;
	
	if (arch==arch_65816) asinit_65816();
	if (arch==arch_spc700) asinit_spc700();
	if (arch==arch_spc700_inline) asinit_spc700();
	if (arch==arch_superfx) asinit_superfx();
	
	warnxkas=false;
	emulatexkas=false;
}


//extern lightweight_map<string, string> defines;
//void nerf(const string& left, string& right){puts(S left+" = "+right);}

void finishpass()
{
//defines.traverse(nerf);
	if (pushpcnum && pass==0) nullerror("pushpc without matching pullpc");
	freespaceend();
	if (arch==arch_65816) asend_65816();
	if (arch==arch_spc700) asend_spc700();
	if (arch==arch_spc700_inline) asend_spc700();
	if (arch==arch_superfx) asend_superfx();
}

bool addlabel(const char * label, int pos=-1)
{
	if (!label[0] || label[0]==':') return false;//colons are reserved for special labels
	if (label[0]=='-' || label[0]=='+')
	{
		int i;
		for (i=0;label[i];i++) if (label[i]!=label[0]) error(0, "Broken label definition");
		if (label[0]=='+') setlabel(S":pos_"+dec(i)+"_"+dec(poslabels[i]++), pos);
		else               setlabel(S":neg_"+dec(i)+"_"+dec(++neglabels[i]), pos);
		return true;
	}
	if (label[strlen(label)-1]==':' || label[0]=='.' || label[0]=='?')
	{
		if (!label[1]) return false;
		bool requirecolon=(label[0]!='.');
		string name=labelname(&label, label[0]!='?');
		if (label[0]==':') label++;
		else if (requirecolon) error(0, "Broken label definition");
		if (label[0]) error(0, "Broken label definition");
		if (ns) name=S ns+"_"+name;
		setlabel(name, pos);
		return true;
	}
	return false;
}

autoarray<bool> elsestatus;
int numtrue=0;//if 1 -> increase both
int numif=0;  //if 0 or inside if 0 -> increase only numif

extern bool asarverallowed;
extern bool istoplevel;

extern bool moreonline;

void print(const char * str);

extern bool checksum;

extern const char * callerfilename;
extern int callerline;
extern int macrorecursion;

int freespaceuse=0;

void assembleblock(const char * block)
{
	string tmp=block;
	int numwords;
	char ** word=qsplit(tmp.str, " ", &numwords);
	autoptr<char**> wordcopy=word;
	while (numif==numtrue && word[0] && addlabel(word[0]))
	{
		word++;
		numwords--;
	}
	if (!word[0] || !word[0][0]) return;
	if (numif==numtrue && word[0][0]=='%')
	{
		if (!macrorecursion)
		{
			callerfilename=thisfilename;
			callerline=thisline;
		}
		callmacro(strchr(block, '%')+1);
		if (!macrorecursion)
		{
			callerfilename=NULL;
			callerline=-1;
		}
		return;
	}
#define is(test) (!strcasecmp(word[0], test))
#define is0(test) (!strcasecmp(word[0], test) && numwords==1)
#define is1(test) (!strcasecmp(word[0], test) && numwords==2)
#define is2(test) (!strcasecmp(word[0], test) && numwords==3)
#define is3(test) (!strcasecmp(word[0], test) && numwords==4)
#define is4(test) (!strcasecmp(word[0], test) && numwords==5)
#define is5(test) (!strcasecmp(word[0], test) && numwords==6)
#define par word[1]
	if (is("if") || is("elseif") || is("assert"))
	{
		if (emulatexkas) warn0("Convert the patch to native Asar format instead of making an Asar-only xkas patch.");
		const char * errmsg=NULL;
		if (is("assert"))
		{
			char * rawerrmsg=strchr(word[numwords-1], ',');
			if (rawerrmsg)
			{
				*rawerrmsg=0;
				errmsg=dequote(rawerrmsg+1);
			}
		}
		if (numtrue!=numif && !(is("elseif") && numtrue+1==numif))
		{
			if (is("if") && !moreonline) numif++;
			return;
		}
		if (is("if") && !moreonline) numif++;
		bool cond;
		
		char ** nextword=word+1;
		char * condstr=NULL;
		while (true)
		{
			if (!nextword[0]) error(0, S"Broken "+lower(word[0])+" command");
			bool thiscond;
			if (!nextword[1] || !strcmp(nextword[1], "&&") || !strcmp(nextword[1], "||"))
			{
				if (nextword[0][0]=='!')
				{
					//this part is not mentioned in the manual
					int val=getnum(nextword[0]+1);
					if (foundlabel) error(1, S"Label in "+lower(word[0])+" command");
					thiscond=!(val>0);
				}
				else
				{
					int val=getnum(nextword[0]);
					if (foundlabel) error(1, S"Label in "+lower(word[0])+" command");
					thiscond=(val>0);
				}
				
				if (condstr && nextword[1])
				{
					if (strcmp(condstr, nextword[1])) error(1, "Invalid condition");
				}
				else condstr=nextword[1];
				nextword+=2;
			}
			else
			{
				if (!nextword[2]) error(0, S"Broken "+lower(word[0])+" command");
				int par1=getnum(nextword[0]);
				if (foundlabel) error(1, S"Label in "+lower(word[0])+" command");
				int par2=getnum(nextword[2]);
				if (foundlabel) error(1, S"Label in "+lower(word[0])+" command");
				if(0);
				else if (!strcmp(nextword[1], ">"))  thiscond=(par1>par2);
				else if (!strcmp(nextword[1], "<"))  thiscond=(par1<par2);
				else if (!strcmp(nextword[1], ">=")) thiscond=(par1>=par2);
				else if (!strcmp(nextword[1], "<=")) thiscond=(par1<=par2);
				else if (!strcmp(nextword[1], "="))  thiscond=(par1==par2);
				else if (!strcmp(nextword[1], "==")) thiscond=(par1==par2);
				else if (!strcmp(nextword[1], "!=")) thiscond=(par1!=par2);
				//else if (!strcmp(nextword[1], "<>")) thiscond=(par1!=par2);
				else error(0, S"Broken "+lower(word[0])+" command");
				
				if (condstr && nextword[3])
				{
					if (strcmp(condstr, nextword[3])) error(1, "Invalid condition");
				}
				else condstr=nextword[3];
				nextword+=4;
			}
			if (condstr)
			{
				if (!strcmp(condstr, "&&") && thiscond) continue;
				if (!strcmp(condstr, "||") && !thiscond) continue;
			}
			cond=thiscond;
			break;
		}
		//if (numwords==4)
		//{
		//	int par1=getnum(word[1]);
		//	if (foundlabel) error(1, S"Label in "+lower(word[0])+" command");
		//	int par2=getnum(word[3]);
		//	if (foundlabel) error(1, S"Label in "+lower(word[0])+" command");
		//	if(0);
		//	else if (!strcmp(word[2], ">"))  cond=(par1>par2);
		//	else if (!strcmp(word[2], "<"))  cond=(par1<par2);
		//	else if (!strcmp(word[2], ">=")) cond=(par1>=par2);
		//	else if (!strcmp(word[2], "<=")) cond=(par1<=par2);
		//	else if (!strcmp(word[2], "="))  cond=(par1==par2);
		//	else if (!strcmp(word[2], "==")) cond=(par1==par2);
		//	else if (!strcmp(word[2], "!=")) cond=(par1!=par2);
		//	//else if (!strcasecmp(word[2], "<>")) cond=(par1!=par2);
		//	else error(0, S"Broken "+lower(word[0])+" command");
		//}
		//else if (*par=='!')
		//{
		//	int val=getnum(par+1);
		//	if (foundlabel) error(1, "Label in if or assert command");
		//	cond=!(val>0);
		//}
		//else
		//{
		//	int val=getnum(par);
		//	if (foundlabel) error(1, "Label in if or assert command");
		//	cond=(val>0);
		//}
		
		if (is("if"))
		{
			if(0);
			else if (cond && moreonline) {}
			else if (cond && !moreonline)
			{
				numtrue++;
				elsestatus[numif]=true;
			}
			else if (!cond && moreonline) moreonline=false;
			else if (!cond && !moreonline)
			{
				elsestatus[numif]=false;
			}
		}
		else if (is("elseif"))
		{
			if (moreonline) error(1, "Can't use elseif on single-line statements");
			if (numif==numtrue) numtrue--;
			if (cond && !elsestatus[numif])
			{
				numtrue++;
				elsestatus[numif]=true;
			}
		}
		else if (!cond)
		{
			if (errmsg) error(1, S"Assertion failed: "+(const char*)errmsg);
			else error(1, "Assertion failed");
		}
	}
	else if (is0("endif"))
	{
		if (!numif) error(1, "Misplaced endif");
		if (numif==numtrue) numtrue--;
		numif--;
	}
//autoarray<bool> elsestatus;
//int numtrue=0;//if 1 -> increase both
//int numif=0;  //if 0 or inside if 0 -> increase only numif
	else if (is0("else"))
	{
		if (!numif) error(1, "Misplaced else");
		else if (numif==numtrue) numtrue--;
		else if (numif==numtrue+1 && !elsestatus[numif])
		{
			numtrue++;
			elsestatus[numif]=true;
		}
	}
	else if (numif!=numtrue) return;
	else if (asblock_pick(word, numwords)) { numopcodes++; }
	else if (is0("error"))
	{
		error(0, "error command encountered");
	}
	else if (is0("warn"))
	{
		if (pass==2) warn("warn command encountered");
	}
	else if (is1("error"))
	{
		error(0, dequote(par));
	}
	else if (is1("warn"))
	{
		if (pass==2) warn(dequote(par));
	}
	else if (is0("@asar") || is1("@asar"))
	{
		if (!asarverallowed) error(0, "This command may only be used at the start of a file.");
		if (!par) return;
		if (emulatexkas) error(0, "This file is incompatible with xkas emulation mode.");
		int dots=0;
		int dig=0;
		for (int i=0;par[i];i++)
		{
			if (par[i]=='.')
			{
				if (!dig) error(0, "Invalid version number");
				dig=0;
				dots++;
			}
			else if (isdigit(par[i])) dig++;
			else error(0, "Invalid version number");
		}
		if (!dig || !dots || dots>2) error(0, "Invalid version number");
		autoptr<char**> vers=split(par, ".");
		int vermaj=atoi(vers[0]);
		if (vermaj>asarver_maj) fatalerror("This version of Asar is too old for this patch.");
		if (vermaj<asarver_maj) return;
		if (dots==1)
		{
			if (strlen(vers[1])!=2) error(0, "Invalid version number.");
			//if (asarver_min<10 && asarver_bug<10 && strlen(vers[1])>2) error(0, "This version of Asar is too old for this patch.");
			int verminbug=atoi(vers[1]);
			int tmpver=asarver_bug;
			if (tmpver>9) tmpver=9;
			if (asarver_min*10+tmpver<verminbug) fatalerror("This version of Asar is too old for this patch.");
		}
		else
		{
			int vermin=atoi(vers[1]);
			if (vermin>asarver_min) fatalerror("This version of Asar is too old for this patch.");
			int verbug=atoi(vers[2]);
			if (vermin==asarver_min && verbug>asarver_bug) fatalerror("This version of Asar is too old for this patch.");
		}
	}
	else if (is0("@xkas"))
	{
		if (!asarverallowed) error(0, "This command may only be used at the start of a file.");
		if (incsrcdepth!=1 && !emulatexkas) error(0, "This command may only be used in the root file.");
		emulatexkas=true;
		optimizeforbank=0x100;
		checksum=false;
		sublabels[0]=":xkasdefault:";
	}
	else if (is0("@include") || is1("@includefrom"))
	{
		if (!asarverallowed) error(0, "This command may only be used at the start of a file.");
		if (istoplevel)
		{
			if (par) fatalerror(S"This file may not be used as the main file. The main file is \""+S par+"\".");
			else fatalerror("This file may not be used as the main file.");
		}
	}
	else if (is1("db") || is1("dw") || is1("dl") || is1("dd"))
	{
		int len;
		if (!confirmqpar(par)) error(0, "Mismatched parentheses");
		if (is1("db")) len=1;
		if (is1("dw")) len=2;
		if (is1("dl")) len=3;
		if (is1("dd")) len=4;
		autoptr<char**> pars=qpsplit(par, ",");
		for (int i=0;pars[i];i++)
		{
			if (pars[i][0]=='"')
			{
				if (!strcmp(pars[i],"\"STAR\"") && !emulatexkas)
						warn0("If you want to assemble an xkas patch, add ;@xkas at the top or you may run into a couple of problems.");
				for (unsigned char * str=(unsigned char*)dequote(pars[i]);*str;str++)
				{
					if (len==1) write1(table[*str]);
					if (len==2) write2(table[*str]);
					if (len==3) write3(table[*str]);
					if (len==4) write4(table[*str]);
				}
			}
			else
			{
				const char * math=pars[i];
				if (math[0]=='#') math++;
				int num=(pass!=0)?getnum(math):0;
				if (len==1) write1(num);
				if (len==2) write2(num);
				if (len==3) write3(num);
				if (len==4) write4(num);
			}
		}
	}
	else if (numwords==3 && !strcasecmp(word[1], "="))
	{
		if (word[0][0]=='\'' && word[0][1] && word[0][2]=='\'' && word[0][3]=='\0')
		{
			table[(unsigned char)word[0][1]]=getnum(word[2]);
			return;
		}
		int num=getnum(word[2]);
		if (foundlabel) error(0, "Setting labels to each other is not valid");
		if (!confirmname(word[0])) error(0, "Invalid label name");
		setlabel(word[0], num);
	}
	else if (is1("org"))
	{
		freespaceend();
		int num=getnum(par);
		if (forwardlabel) error(0, "org Label is only valid for labels earlier in the patch");
		if (num&~0xFFFFFF) error(1, "Address out of bounds");
		if (mapper==lorom && (num&0x408000)==0x400000) warn0("It would be wise to set the 008000 bit of this address.");
		//if (fastrom) num|=0x800000;
		snespos=num;
		realsnespos=num;
		startpos=num;
		realstartpos=num;
	}
	else if (is1("base"))
	{
		if (!strcasecmp(par, "off"))
		{
			snespos=realsnespos;
			startpos=realstartpos;
			return;
		}
		int num=getnum(par);
		if (forwardlabel) error(0, "base Label is not valid");
		if (num&~0xFFFFFF) error(1, "Address out of bounds");
		snespos=num;
		startpos=num;
		optimizeforbank=-1;
	}
	else if (is1("bank"))
	{
		if (!strcasecmp(par, "auto"))
		{
			optimizeforbank=-1;
			return;
		}
		if (!strcasecmp(par, "noassume"))
		{
			optimizeforbank=0x100;
			return;
		}
		int num=getnum(par);
		//if (forwardlabel) error(0, "bank Label is not valid");
		//if (foundlabel) num>>=16;
		if (num&~0x0000FF) error(1, "Address out of bounds");
		optimizeforbank=num;
	}
	else if (is("freespace") || is("freecode") || is("freedata"))
	{
		if (emulatexkas) warn0("Convert the patch to native Asar format instead of making an Asar-only xkas patch.");
		string parstr;
		if (numwords==1) parstr="\n";//crappy hack: impossible character to cut out extra commas
		else if (numwords==2) parstr=word[1];
		else error(0, "Invalid freespace request.");
		if (is("freecode")) parstr=S"ram,"+parstr;
		if (is("freedata")) parstr=S"noram,"+parstr;
		autoptr<char**> pars=split(parstr.str, ",");
		int useram=-1;
		bool fixedpos=false;
		bool align=false;
		bool leakwarn=true;
		for (int i=0;pars[i];i++)
		{
			if (pars[i][0]=='\n') {}
			else if (!strcasecmp(pars[i], "ram"))
			{
				if (useram!=-1) error(0, "Invalid freespace request.");
				useram=1;
			}
			else if (!strcasecmp(pars[i], "noram"))
			{
				if (useram!=-1) error(0, "Invalid freespace request.");
				useram=0;
			}
			else if (!strcasecmp(pars[i], "static") || !strcasecmp(pars[i], "fixed"))
			{
				if (fixedpos) error(0, "Invalid freespace request.");
				fixedpos=true;
			}
			else if (!strcasecmp(pars[i], "align"))
			{
				if (align) error(0, "Invalid freespace request.");
				align=true;
			}
			else if (!strcasecmp(pars[i], "cleaned"))
			{
				if (!leakwarn) error(0, "Invalid freespace request.");
				leakwarn=false;
			}
			else error(0, "Invalid freespace request.");
		}
		if (useram==-1) error(0, "Invalid freespace request.");
		if (mapper==hirom && useram) error(0, "No banks contain the RAM mirrors in hirom.");
		if (mapper==norom) error(0, "Can't find freespace in norom.");
		freespaceend();
		freespaceid=getfreespaceid();
		if (pass==0) snespos=(freespaceid<<24)|0x8000;
		if (pass==1)
		{
			if (fixedpos && freespaceorgpos[freespaceid]<0)
			{
				freespacepos[freespaceid]=0x008000;
				freespaceleak[freespaceid]=false;//mute some other errors
				/*if (freespaceorgpos[freespaceid]==-1)*/ error(1, "A static freespace must be targeted by at least one autoclean.");
				/*if (freespaceorgpos[freespaceid]==-2)   error(1, "A static freespace must be targeted by at least one autoclean.");*/
			}
			if (fixedpos && freespaceorgpos[freespaceid]) freespacepos[freespaceid]=snespos=(freespaceid<<24)|freespaceorgpos[freespaceid];
			else freespacepos[freespaceid]=snespos=(freespaceid<<24)|getsnesfreespace(freespacelen[freespaceid], useram, true, true, align);
		}
		if (pass==2)
		{
			if (fixedpos && freespaceorgpos[freespaceid]==-1) return;//to kill some errors
			snespos=(freespaceid<<24)|freespacepos[freespaceid];
			resizerats(snespos, freespacelen[freespaceid]);
			if (freespaceleak[freespaceid] && leakwarn) warn2("This freespace appears to be leaked.");
			if (fixedpos && freespaceorgpos[freespaceid]>0 && freespacelen[freespaceid]>freespaceorglen[freespaceid])
					error(2, "A static freespace may not grow.");
			freespaceuse+=8+freespacelen[freespaceid];
		}
		freespacestatic[freespaceid]=fixedpos;
		if (snespos<0 && mapper==sa1rom) fatalerror(S"No freespace found in the mapped banks (requested size: "+dec(freespacelen[freespaceid])+")");
		if (snespos<0) fatalerror(S"No freespace found (requested size: "+dec(freespacelen[freespaceid])+")");
		bytes+=8;
		//if (fastrom) snespos|=0x800000;
		freespacestart=snespos;
		startpos=snespos;
		realstartpos=snespos;
		realsnespos=snespos;
		optimizeforbank=-1;
		ratsmetastate=ratsmeta_allow;
	}
	else if (is1("prot"))
	{
		if (!confirmqpar(par)) error(0, "Mismatched parentheses");
		if (!ratsmetastate) error(2, "PROT must be used at the start of a freespace block.");
		//int here=(((realsnespos&0xFFFFFF)&~0x8000)-8)|0x8000;
		if (ratsmetastate==ratsmeta_used) step(-5);
		int num;
		autoptr<char**> pars=qpsplit(par, ",", &num);
		write1('P');
		write1('R');
		write1('O');
		write1('T');
		if (num*3>255) error(0, "Too many entries to PROT.");
		write1(num*3);
		for (int i=0;i<num;i++)
		{
			//int num=getnum(pars[i]);
			const char * labeltest=pars[i];
			int num=labelval(&labeltest);
			if (*labeltest) error(0, "Not a label.");
			write3(num);
			if (pass==1) freespaceleak[num>>24]=false;
		}
		write1('S');
		write1('T');
		write1('O');
		write1('P');
		write1(0);
		ratsmetastate=ratsmeta_used;
	}
	else if (is1("autoclean") || is2("autoclean") || is1("autoclear") || is2("autoclear"))
	{
		if (numwords==3)
		{
			if (snespos&0xFF000000) error(0, "autoclean used in freespace");
			const char * labeltest=word[2];
			int num=labelval(&labeltest);
			if (*labeltest) error(0, "Not a label.");
			unsigned char targetid=num>>24;
			if (pass==1) freespaceleak[targetid]=false;
			num&=0xFFFFFF;
			if (strlen(par)>3 && !strcasecmp(par+3, ".l")) par[3]=0;
			if (!strcasecmp(par, "JSL") || !strcasecmp(par, "JML"))
			{
				int orgpos=read3(snespos+1);
				int ratsloc=freespaceorgpos[targetid];
				int firstbyte=((!strcasecmp(par, "JSL"))?0x22:0x5C);
				int pcpos=snestopc(snespos);
				if (/*pass==1 && */pcpos>=0 && pcpos<romlen_r && romdata_r[pcpos]==firstbyte)
				{
					ratsloc=ratsstart(orgpos)+8;
					freespaceorglen[targetid]=read2(ratsloc-4)+1;
					if (!freespacestatic[targetid] && pass==1) removerats(orgpos);
				}
				else if (ratsloc<0) ratsloc=0;
				write1(firstbyte);
				write3(num);
				if (pass==2)
				{
					int start=ratsstart(num);
					if (start>=num || start<0)
						error(2, "Don't autoclean a label at the end of a freespace block, you'll remove some stuff you're not supposed to remove.");
				}
				//freespaceorglen[targetid]=read2(ratsloc-4)+1;
				freespaceorgpos[targetid]=ratsloc;
			}
			else if (!strcasecmp(par, "dl"))
			{
				int orgpos=read3(snespos);
				int ratsloc=freespaceorgpos[targetid];
				int start=ratsstart(num);
				if (pass==1 && num>=0)
				{
					ratsloc=ratsstart(orgpos)+8;
					if (!freespacestatic[targetid]) removerats(orgpos);
				}
				else if (!ratsloc) ratsloc=0;
				if ((start==num || start<0) && pass==2)
					error(2, "Don't autoclean a label at the end of a freespace block, you'll remove some stuff you're not supposed to remove.");
				write3(num);
				freespaceorgpos[targetid]=ratsloc;
				freespaceorglen[targetid]=read2(ratsloc-4)+1;
			}
			else error(0, "Broken autoclean command");
		}
		else if (pass==0) removerats(getnum(word[1]));
	}
	else if (is0("pushpc"))
	{
		if (emulatexkas) warn0("Convert the patch to native Asar format instead of making an Asar-only xkas patch.");
		pushpc[pushpcnum].arch=arch;
		pushpc[pushpcnum].snespos=snespos;
		pushpc[pushpcnum].snesstart=startpos;
		pushpc[pushpcnum].snesposreal=realsnespos;
		pushpc[pushpcnum].snesstartreal=realstartpos;
		pushpc[pushpcnum].freeid=freespaceid;
		pushpc[pushpcnum].freeex=freespaceextra;
		pushpc[pushpcnum].freest=freespacestart;
		pushpcnum++;
		snespos=0xFFFFFFFF;
		startpos=0xFFFFFFFF;
		realsnespos=0xFFFFFFFF;
		realstartpos=0xFFFFFFFF;
	}
	else if (is0("pullpc"))
	{
		if (!pushpcnum) error(0, "pullpc without matching pushpc");
		pushpcnum--;
		freespaceend();
		if (arch!=pushpc[pushpcnum].arch) error(0, "pullpc in another architecture than the pushpc");
		snespos=pushpc[pushpcnum].snespos;
		startpos=pushpc[pushpcnum].snesstart;
		realsnespos=pushpc[pushpcnum].snesposreal;
		realstartpos=pushpc[pushpcnum].snesstartreal;
		freespaceid=pushpc[pushpcnum].freeid;
		freespaceextra=pushpc[pushpcnum].freeex;
		freespacestart=pushpc[pushpcnum].freest;
	}
	else if (is1("namespace"))
	{
		if (par)
		{
			if (!strcasecmp(par, "off")) ns="";
			else
			{
				const char * tmp=dequote(par);
				if (!confirmname(tmp)) error(0, "Bad namespace name");
				ns=tmp;
			}
		}
		else ns="";
	}
	else if (is1("warnpc"))
	{
		int maxpos=getnum(par);
		if (snespos&0xFF000000) error(0, "warnpc used in freespace");
		if (maxpos&0xFF000000) error(0, "Broken warnpc argument");
		if (snespos>maxpos) error(0, S"warnpc failed: Current position ("+hex6(snespos)+") is after end position ("+hex6(maxpos)+")");
		if (warnxkas && snespos==maxpos) warn0(S"xkas conversion warning: warnpc is relaxed one byte in Asar");
		if (emulatexkas && snespos==maxpos) error(0, S"warnpc failed: Current position ("+hex6(snespos)+") is equal to end position ("+hex6(maxpos)+")");
	}
	else if (is1("rep"))
	{
		int rep=getnum(par);
		if (foundlabel) error(0, "rep Label is not valid");
		if (rep<=1)
		{
			if (emulatexkas)
			{
				if (rep==0) rep=1;
				if (rep<0) rep=0;
				repeatnext=rep;
				return;
			}
			warn0("xkas-style conditional compilation detected. Please use the if command instead.");
		}
		repeatnext=rep;
	}
#ifdef SANDBOX
	else if (is("incsrc") || is("incbin") || is("table"))
	{
		error(0, "This command is disabled.");
	}
#endif
	else if (is1("incsrc"))
	{
		checkbankcross();
		string name;
		if (warnxkas && (strchr(thisfilename, '/') || strchr(thisfilename, '\\')))
			warn0("xkas compatibility warning: incsrc and incbin look for files relative to the patch in Asar, "
					"but xkas looks relative to the assembler.");
		if (strchr(par, '\\'))
		{
			if (emulatexkas)
			{
				for (int i=0;par[i];i++)
				{
					if (par[i]=='\\') par[i]='/';//let's just hope nobody finds I could just enable this for everything.
				}
			}
#ifdef _WIN32
			else warn0("This patch may not assemble cleanly on all platforms. Please use / instead.");
#endif
		}
		if (emulatexkas) name=dequote(par);
		else name=S dir(thisfilename)+dequote(par);
		assemblefile(name, false);
	}
	else if (is1("incbin") || is3("incbin"))
	{
		if (numwords==4 && strcmp(word[2], "->")) error(0, "Broken incbin command");
		int len;
		int start=0;
		int end=0;
		if (strqchr(par, ':'))
		{
			char * lengths=strqchr(par, ':');
			*lengths=0;
			lengths++;
			if (strchr(lengths, '"')) error(0, "Broken incbin command");
			start=strtoul(lengths, &lengths, 16);
			if (*lengths!='-') error(0, "Broken incbin command");
			lengths++;
			end=strtoul(lengths, &lengths, 16);
			if (*lengths) error(0, "Broken incbin command");
		}
		string name;
		if (warnxkas && (strchr(thisfilename, '/') || strchr(thisfilename, '\\')))
			warn0("xkas compatibility warning: incsrc and incbin look for files relative to the patch in native mode, "
					"but relative to the assembler in emulation mode");
		if (strchr(par, '\\'))
		{
			if (emulatexkas)
			{
				for (int i=0;par[i];i++)
				{
					if (par[i]=='\\') par[i]='/';//let's just hope nobody finds I could just enable this for everything.
				}
			}
#ifdef _WIN32
			else warn0("This patch may not assemble cleanly on all platforms. Please use / instead.");
#endif
		}
		if (emulatexkas) name=dequote(par);
		else name=S dir(thisfilename)+dequote(par);
		char * data;//I couldn't find a way to get this into an autoptr
		if (!readfile(name, &data, &len)) error(0, "File not found");
		autoptr<char*> datacopy=data;
		if (!end) end=len;
		if (end<start) error(0, "Negative range in incbin");
		if (end>len) error(0, "The file is not that long.");
		if (numwords==4)
		{
			if (!confirmname(word[3]))
			{
				int pos=getnum(word[3]);
				if (foundlabel) error(0, "Can't use labels here.");
				int offset=snestopc(pos);
				if (offset+end-start>0xFFFFFF) error(0, "Can't create ROMs larger than 16MB");
				if (offset+end-start>romlen) romlen=offset+end-start;
				if (pass==2) memcpy(romdata+offset, data+start, end-start);
			}
			else
			{
				int pos;
				if (pass==0)
				{
					if (end-start>65536) error(0, "Can't include more than 64 kilobytes at once");
					pos=getpcfreespace(end-start, false, true, false);
					if (pos<0) error(0, "No freespace found");
					int freespaceid=getfreespaceid();
					freespacepos[freespaceid]=pctosnes(pos)|(/*fastrom?0x800000:*/0x000000)|(freespaceid<<24);
					setlabel(word[3], freespacepos[freespaceid]);
					memset(romdata+pos, 0xFF, end-start);
				}
				if (pass==1)
				{
					getfreespaceid();//nothing to do here, but we want to tick the counter
				}
				if (pass==2)
				{
					int freespaceid=getfreespaceid();
					if (freespaceleak[freespaceid]) warn2("This freespace appears to be leaked.");
					memcpy(romdata+snestopc(freespacepos[freespaceid]&0xFFFFFF), data+start, end-start);
					freespaceuse+=8+end-start;
				}
			}
		}
		else
		{
			for (int i=start;i<end;i++) write1(data[i]);
		}
	}
	else if (is1("skip"))//nobody ever uses this, but whatever
	{
		int skip=getnum(par);
		if (foundlabel) error(0, "skip Label is not valid");
		step(skip);
	}
	else if (is0("cleartable"))
	{
		cleartable();
	}
	else if (is1("table"))
	{
		bool fliporder=false;
		if(0);
		else if (striend(par, ",ltr")) { itrim(par, "", ",ltr"); }
		else if (striend(par, ",rtl")) { itrim(par, "", ",rtl"); fliporder=true; }
		string name=S dir(thisfilename)+dequote(par);
		autoptr<char*> tablecontents=readfile(name);
		if (!tablecontents) error(0, "File not found");
		autoptr<char**> tablelines=split(tablecontents, "\n");
		for (int i=0;i<256;i++) table[i]=((numopcodes+read2(0x00FFDE)+i)*0x26594131)|0x40028020;
			//garbage value so people will notice they're doing it wrong (for bonus points: figure out what 0x26594131 is)
		for (int i=0;tablelines[i];i++)
		{
			string tableline=tablelines[i];
			if (!*tableline) continue;
			if (strlen(tableline)<4 || strlen(tableline)&1 || strlen(tableline)>10) error(0, "Invalid table file");
			if (!fliporder)
			{
				if (tableline[3]=='x' || tableline[3]=='X') error(0, "Invalid table file");
				char * end;
				table[(unsigned char)tableline[0]]=strtol(tableline+2, &end, 16);
				if (*end) error(0, "Invalid table file");
			}
			else
			{
				if (tableline[1]=='x' || tableline[1]=='X') error(0, "Invalid table file");
				char * eq;
				unsigned int val=strtol(tableline, &eq, 16);
				if (eq[0]!='=' || eq[2]) error(0, "Invalid table file");
				table[(unsigned char)eq[1]]=val;
			}
			//if (strlen(tableline)!=4 || tableline[eqoff]!='=' || !ctype_xdigit(tableline+hexoff, 2)) error(0, "Invalid table file");
			//table[(unsigned char)tableline[charoff]]=strtol(tableline+hexoff, NULL, 16);
			//autoptr<char**> words=split(tablelines[i], "=");
			//string ch=fliporder?words[1]:words[0];
			//string val=fliporder?words[0]:words[1];
			//bool space=(*ch==' ');
			//clean(ch);
			//clean(val);
			//if (space && !*ch) ch=" ";
			//if (strlen(ch)!=1 || strlen(val)>8) error(0, "Invalid table file");
			//char * endtest;
			//table[(unsigned char)*ch]=strtol(val, &endtest, 16);
			//if (*endtest) error(0, "Invalid table file");
		}
	}
	else if (is3("function"))
	{
		if (!pass)
		{
			if (strcasecmp(word[2], "=")) error(0, "Broken function declaration");
			if (!confirmqpar(word[1])) error(0, "Broken function declaration");
			string line=word[1];
			clean(line);
			char * startpar=strqchr(line.str, '(');
			if (!startpar) error(0, "Broken function declaration");
			*startpar=0;
			startpar++;
			if (!confirmname(line)) error(0, "Bad function name");
			char * endpar=strqchr(startpar, ')');
			//confirmqpar requires that all parentheses are matched, and a starting one exists, therefore it is harmless to not check for nulls
			if (endpar[1]) error(0, "Broken function declaration");
			*endpar=0;
			const char * e=createuserfunc(line, startpar, word[3]);
			if (e) error(0, e);
		}
	}
	else if (is1("print"))
	{
		if (!confirmqpar(par)) error(0, "Mismatched parentheses");
		string out;
		autoptr<char**> pars=qpsplit(par, ",");
		for (int i=0;pars[i];i++)
		{
			if(0);
			else if (pars[i][0]=='"') out+=dequote(pars[i]);
			else if (!strcasecmp(pars[i], "bytes")) out+=dec(bytes);
			else if (!strcasecmp(pars[i], "freespaceuse")) out+=dec(freespaceuse);
			else if (!strcasecmp(pars[i], "pc")) out+=hex6(snespos&0xFFFFFF);
			else if (!strncasecmp(pars[i], "dec(", strlen("dec("))) out+=dec(getnum(pars[i]+strlen("dec")));
			else if (!strncasecmp(pars[i], "hex(", strlen("hex("))) out+=hex0(getnum(pars[i]+strlen("hex")));
			else error(2, "Unknown variable.");
		}
		if (pass!=2) return;
		print(out);
	}
	else if (is1("reset"))
	{
		if(0);
		else if (!strcasecmp(par, "bytes")) bytes=0;
		else if (!strcasecmp(par, "freespaceuse")) freespaceuse=0;
		else error(2, "Unknown variable.");
	}
	else if (is1("padbyte") || is1("padword") || is1("padlong") || is1("paddword"))
	{
		int len;
		if (is("padbyte")) len=1;
		if (is("padword")) len=2;
		if (is("padlong")) len=3;
		if (is("paddword")) len=4;
		unsigned int val=getnum(par);
		for (int i=0;i<12;i+=len)
		{
			unsigned int tmpval=val;
			for (int j=0;j<len;j++)
			{
				padbyte[i+j]=tmpval;
				tmpval>>=8;
			}
		}
	}
	else if (is1("pad"))
	{
		if (snespos&0xFF000000) error(0, "pad does not make sense in a freespaced code");
		int num=getnum(par);
		if (num&0xFF000000) error(0, "Out of bounds");
		if (num>snespos)
		{
			int end=snestopc(num);
			int start=snestopc(snespos);
			int len=end-start;
			for (int i=0;i<len;i++) romdata[start+i]=padbyte[i%12];
			snespos=num;
		}
	}
	else if (is1("fillbyte") || is1("fillword") || is1("filllong") || is1("filldword"))
	{
		int len;
		if (is("fillbyte")) len=1;
		if (is("fillword")) len=2;
		if (is("filllong")) len=3;
		if (is("filldword")) len=4;
		unsigned int val=getnum(par);
		for (int i=0;i<12;i+=len)
		{
			unsigned int tmpval=val;
			for (int j=0;j<len;j++)
			{
				fillbyte[i+j]=tmpval;
				tmpval>>=8;
			}
		}
	}
	else if (is1("fill"))
	{
		int num=getnum(par);
		for (int i=0;i<num;i++) write1(fillbyte[i%12]);
	}
	else if (is1("arch"))
	{
		if (emulatexkas) warn0("Convert the patch to native Asar format instead of making an Asar-only xkas patch.");
		if (!strcasecmp(par, "65816")) { arch=arch_65816; return; }
		if (!strcasecmp(par, "spc700")) { arch=arch_spc700; return; }
		if (!strcasecmp(par, "spc700-inline")) { arch=arch_spc700_inline; return; }
		if (!strcasecmp(par, "spc700-raw")) { arch=arch_spc700; mapper=norom; checksum=false; return; }
		if (!strcasecmp(par, "superfx")) { arch=arch_superfx; return; }
	}
	else if (is2("math"))
	{
		bool val;
		if(0);
		else if (!strcasecmp(word[2], "on")) val=true;
		else if (!strcasecmp(word[2], "off")) val=false;
		else error(0, "Invalid math command.");
		if(0);
		else if (!strcasecmp(word[1], "pri")) math_pri=val;
		else if (!strcasecmp(word[1], "round")) math_round=val;
		else error(0, "Invalid math command.");
	}
	else if (is2("warn"))
	{
		bool val;
		if(0);
		else if (!strcasecmp(word[2], "on")) val=true;
		else if (!strcasecmp(word[2], "off")) val=false;
		else error(0, "Invalid warn command.");
		if(0);
		else if (!strcasecmp(word[1], "xkas")) warnxkas=val;
		else error(0, "Invalid warn command.");
	}
	else if (is0("fastrom"))
	{
		//removed due to causing more trouble than it's worth.
		//if (emulatexkas) warn0("Convert the patch to native Asar format instead of making an Asar-only xkas patch.");
		//if (mapper==lorom || mapper==hirom) fastrom=true;
		//else error(0, "Can't use fastrom in this mapper.");
	}
	else if (is0("{") || is0("}")) {}
	else error(1, "Unknown command.");
}

bool assemblemapper(char** word, int numwords)
{
	if(0);
	else if (is0("lorom"))
	{
		//this also makes xkas set snespos to $008000 for some reason
		mapper=lorom;
	}
	else if (is0("hirom"))
	{
		//xkas makes this point to $C00000
		mapper=hirom;
	}
	else if (is0("sfxrom"))
	{
		mapper=sfxrom;
		//fastrom=false;
	}
	else if (is0("norom"))
	{
		//$000000 would be the best snespos for this, but I don't care
		mapper=norom;
		//fastrom=false;
		checksum=false;//we don't know where the header is, so don't set the checksum
	}
	else if (is0("fullsa1rom"))
	{
		mapper=bigsa1rom;
		//fastrom=false;
	}
	else if (is("sa1rom"))
	{
		//fastrom=false;
		if (par)
		{
			if (word[2]) error(0, "Invalid mapper.");
			if (!isdigit(par[0]) || par[1]!=',' ||
					!isdigit(par[2]) || par[3]!=',' ||
					!isdigit(par[4]) || par[5]!=',' ||
					!isdigit(par[6]) || par[7]) error(0, "Invalid mapper.");
			int len;
			autoptr<char**> pars=qpsplit(par, ",", &len);
			if (len!=4) error(0, "Invalid mapper.");
			sa1banks[0]=(par[0]-'0')<<20;
			sa1banks[1]=(par[2]-'0')<<20;
			sa1banks[4]=(par[4]-'0')<<20;
			sa1banks[5]=(par[6]-'0')<<20;
		}
		else
		{
			sa1banks[0]=0<<20;
			sa1banks[1]=1<<20;
			sa1banks[4]=2<<20;
			sa1banks[5]=3<<20;
		}
		mapper=sa1rom;
	}
	else if (is0("header"))
	{
		//headers are detected elsewhere; ignoring for familiarity
	}
	else return false;
	return true;
}
