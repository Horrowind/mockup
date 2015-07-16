#define INTERFACE_LIB

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "libsmw.h"
#include "libstr.h"
#include "scapegoat.hpp"
#include "autoarray.h"
#include "asar.h"

extern const int asarver_maj=1;
extern const int asarver_min=3;
extern const int asarver_bug=6;
extern const bool asarver_beta=false;

#ifdef _I_RELEASE
extern char blockbetareleases[(!asarver_beta)?1:-1];
#endif
#ifdef _I_DEBUG
extern char blockreleasedebug[(asarver_beta)?1:-1];
#endif

unsigned const char * romdata_r;
int romlen_r;

long double math(const char * mystr, const char ** e);
void initmathcore();

int pass;

int optimizeforbank=-1;

const char * thisfilename;
int thisline;
const char * thisblock;

const char * callerfilename=NULL;
int callerline=-1;

bool errored=false;

volatile int recursioncount=0;

unsigned int labelval(const char ** rawname, bool update);
unsigned int labelval(char ** rawname, bool update);
unsigned int labelval(string name, bool define);
bool labelval(const char ** rawname, unsigned int * rval, bool update);
bool labelval(char ** rawname, unsigned int * rval, bool update);
bool labelval(string name, unsigned int * rval, bool define);

void startmacro(const char * line);
void tomacro(const char * line);
void endmacro(bool insert);
void callmacro(const char * data);

bool setmapper()
{
	int maxscore=-99999;
	mapper_t bestmap=lorom;
	mapper_t maps[]={lorom, hirom};
	for (size_t mapid=0;mapid<sizeof(maps)/sizeof(maps[0]);mapid++)
	{
		mapper=maps[mapid];
		int score=0;
		int highbits=0;
		bool foundnull=false;
		for (int i=0;i<21;i++)
		{
			unsigned char c=romdata[snestopc(0x00FFC0+i)];
			if (foundnull && c) score-=4;//according to some documents, NUL terminated names are possible - but they shouldn't appear in the middle of the name
			if (c>=128) highbits++;
			else if (isupper(c)) score+=3;
			else if (c==' ') score+=2;
			else if (isdigit(c)) score+=1;
			else if (islower(c)) score+=1;
			else if (c=='-') score+=1;
			else if (!c) foundnull=true;
			else score-=3;
		}
		if (highbits>0 && highbits<=14) score-=21;//high bits set on some, but not all, bytes = unlikely to be a ROM
		if ((romdata[snestopc(0x00FFDE)]^romdata[snestopc(0x00FFDC)])!=0xFF ||
				(romdata[snestopc(0x00FFDF)]^romdata[snestopc(0x00FFDD)])!=0xFF) score=-99999;//checksum doesn't match up to 0xFFFF? Not a ROM.
		//too lazy to check the real checksum
		if (score>maxscore)
		{
			maxscore=score;
			bestmap=mapper;
		}
	}
	mapper=bestmap;
	
	//detect oddball mappers
	int mapperbyte=romdata[snestopc(0x00FFD5)];
	int romtypebyte=romdata[snestopc(0x00FFD6)];
	if (mapper==lorom)
	{
		if (mapperbyte==0x23 && (romtypebyte==0x32 || romtypebyte==0x34 || romtypebyte==0x35)) mapper=sa1rom;
	}
	return (maxscore>=0);
}

string getdecor()
{
	string e;
	if (thisfilename)
	{
		e+=S thisfilename;
		if (thisline!=-1) e+=S ":"+dec(thisline+1);
		if (callerfilename) e+=S" (called from "+callerfilename+":"+dec(callerline+1)+")";
		e+=": ";
	}
	return e;
}

//modified from somewhere in nall (license: ISC)
string dir(char const *name) {
  string result = name;
  for(signed i = strlen(result); i >= 0; i--) {
    if(result[i] == '/' || result[i] == '\\') {
      result[i + 1] = 0;
      break;
    }
    if(i == 0) result = "";
  }
  return result;
}

//extern lightweight_map<string, unsigned int> labels;
extern autoarray<int> poslabels;
extern autoarray<int> neglabels;

extern int freespaceextra;
extern int freespaceid;
extern int freespacepos[256];

void checkbankcross();

extern bool warnxkas;
extern bool emulatexkas;

static bool freespaced;
static int getlenforlabel(int snespos, int thislabel, bool exists)
{
	if (warnxkas && (((thislabel^snespos)&0xFFFF0000)==0))
		warn1("xkas compatibility warning: label access is always 24bit in emulation mode, but may be 16bit in native mode");
	if (!exists)
	{
		if (!freespaced) freespaceextra++;
		freespaced=true;
		return 2;
	}
	else if (optimizeforbank>=0)
	{
		if (thislabel&0xFF000000) return 3;
		else if ((thislabel>>16)==optimizeforbank) return 2;
		else return 3;
	}
	else if ((thislabel|snespos)&0xFF000000)
	{
		if ((thislabel^snespos)&0xFF000000) return 3;
		else return 2;
	}
	else if ((thislabel^snespos)&0xFF0000) return 3;
	else return 2;
}

int getlen(const char * orgstr, bool optimizebankextraction)
{
	const char * str=orgstr;
	freespaced=false;
	if (str[0]=='+' || str[0]=='-')
	{
		int i;
		for (i=0;str[i];i++)
		{
			if (str[i]!=str[0]) goto notposneglabel;
		}
		if (!pass) return 2;
		unsigned int labelpos=31415926;
		bool found;
		if (str[0]=='+') found=labelval(S":pos_"+dec(i)+"_"+dec(poslabels[i]), &labelpos);
		else             found=labelval(S":neg_"+dec(i)+"_"+dec(neglabels[i]), &labelpos);
		return getlenforlabel(snespos, labelpos, found);
	}
notposneglabel:
	int len=0;
	while (*str)
	{
		int thislen=0;
		bool maybebankextraction=(str==orgstr);
		if (*str=='$')
		{
			str++;
			int i;
			for (i=0;isxdigit(str[i]);i++);
			//if (i&1) warn(S dec(i)+"-digit hex value");//blocked in getnum instead
			thislen=(i+1)/2;
			str+=i;
		}
		else if (*str=='%')
		{
			str++;
			int i;
			for (i=0;str[i]=='0' || str[i]=='1';i++);
			//if (i&7) warn(S dec(i)+"-digit binary value");
			thislen=(i+7)/8;
			str+=i;
		}
		else if (str[0]=='\'' && str[2]=='\'')
		{
			thislen=1;
			str+=3;
		}
		else if (isdigit(*str))
		{
			int val=strtol(str, (char**)&str, 10);
			if (val>=0) thislen=1;
			if (val>=256) thislen=2;
			if (val>=65536) thislen=3;
			if (val>=16777216) thislen=4;
		}
		else if (isalpha(*str) || *str=='.' || *str=='?')
		{
			unsigned int thislabel;
			bool exists=labelval(&str, &thislabel);
			thislen=getlenforlabel(snespos, thislabel, exists);
		}
		else str++;
		if (optimizebankextraction && maybebankextraction &&
				(!strcmp(str, ">>16") || !strcmp(str, "/65536") || !strcmp(str, "/$10000")))
					return 1;
		if (thislen>len) len=thislen;
	}
	if (len>3) return 3;
	return len;
}

extern bool foundlabel;
extern bool forwardlabel;

int getnum(const char * str)
{
//	if (str[0]=='+' || str[0]=='-')
//	{
//		int i;
//		for (i=0;str[i];i++)
//		{
//			if (str[i]!=str[0]) goto notposneglabel;
//		}
//		if (!str[i])
//		{
//			foundlabel=true;
//			if (str[0]=='+') forwardlabel=true;
//			if (str[0]=='+') return labelval(S":pos_"+dec(i)+"_"+dec(poslabels[i]))&0xFFFFFF;
//			else             return labelval(S":neg_"+dec(i)+"_"+dec(neglabels[i]))&0xFFFFFF;
//			return 0;
//		}
//	}
//notposneglabel:
	const char * e;
	int num=math(str, &e);
	if (e)
	{
		error<errblock>(1, e);
	}
	return num;
}

struct strcompare {
	bool operator() (const char * lhs, const char * rhs) const
	{
		return strcmp(lhs, rhs)<0;
	}
};

struct stricompare {
	bool operator() (const char * lhs, const char * rhs) const
	{
		return strcasecmp(lhs, rhs)<0;
	}
};

lightweight_map<string, char**> filecontents;
lightweight_map<string, string> defines;

void assembleblock(const char * block);

void resolvedefines(string& out, const char * start)
{
	recurseblock rec;
	const char * here=start;
	while (*here)
	{
		if (*here=='"' && emulatexkas)
		{
			out+=*here++;
			while (*here && *here!='"') out+=*here++;
			out+=*here++;
		}
		else if (*here=='!')
		{
			bool first=(here==start || (here>=start+4 && here[-1]==' ' && here[-2]==':' && here[-3]==' '));//check if it's the start of a block
			string defname;
			here++;
			if (*here=='{')
			{
				here++;
				string unprocessedname;
				int braces=1;
				while (true)
				{
					if (*here=='{') braces++;
					if (*here=='}') braces--;
					if (!*here) error<errline>(0, "Unmatched braces.");
					if (!braces) break;
					unprocessedname+=*here++;
				}
				here++;
				resolvedefines(defname, unprocessedname);
				bool bad=false;
				if (!defname[0]) bad=true;
				for (int i=0;defname[i];i++)
				{
					if (!isualnum(defname[i])) bad=true;
				}
				if (bad) error<errline>(0, "Invalid define name.");
			}
			else
			{
				while (isualnum(*here)) defname+=*here++;
			}
			if (warnxkas && here[0]=='(' && here[1]==')')
				warn0("xkas compatibility warning: Unlike xkas, Asar does not eat parentheses after defines");
			//if (emulatexkas && here[0]=='(' && here[1]==')') here+=2;
			if (first)
			{
				enum {
					null,
					append,
					expand,
					domath,
					setifnotset,
				} mode;
				if(0);
				else if (stribegin(here,  " = ")) { here+=3; mode=null; }
				else if (stribegin(here, " += ")) { here+=4; mode=append; }
				else if (stribegin(here, " := ")) { here+=4; mode=expand; }
				else if (stribegin(here, " #= ")) { here+=4; mode=domath; }
				else if (stribegin(here, " ?= ")) { here+=4; mode=setifnotset; }
				else goto notdefineset;
				if (emulatexkas && mode!=null) warn0("Convert the patch to native Asar format instead of making an Asar-only xkas patch.");
				//else if (stribegin(here, " equ ")) here+=5;
				string val;
				if (*here=='"')
				{
					here++;
					while (true)
					{
						if (*here=='"')
						{
							if (!here[1] || here[1]==' ') break;
							else if (here[1]=='"') here++;
							else error<errline>(0, "Broken define declaration");
						}
						val+=*here++;
					}
					here++;
				}
				else
				{
					while (*here && *here!=' ') val+=*here++;
				}
				//if (strqchr(val.str, ';')) *strqchr(val.str, ';')=0;
				if (*here && !stribegin(here, " : ")) error<errline>(0, "Broken define declaration");
				clean(val);
				switch (mode)
				{
					case null:
					{
						defines.insert(defname, val);
						break;
					}
					case append:
					{
						string oldval;
						if (!defines.find(defname, oldval)) error<errnull>(0, "Appending to an undefined define");
						val=oldval+val;
						defines.insert(defname, val);
						break;
					}
					case expand:
					{
						string newval;
						resolvedefines(newval, val);
						defines.insert(defname, newval);
						break;
					}
					case domath:
					{
						string newval;
						resolvedefines(newval, val);
						int num=getnum(newval);
						if (foundlabel) error<errline>(0, "!Define #= Label is not allowed");
						defines.insert(defname, dec(num));
						break;
					}
					case setifnotset:
					{
						string idontcare;
						if (!defines.find(defname, idontcare)) defines.insert(defname, val);
						break;
					}
				}
			}
			else
			{
			notdefineset:
				if (!defname) out+="!";
				else
				{
					string thisone;
					if (!defines.find(defname, thisone)) error<errline>(0, S"Define !"+defname+" not found");
					else resolvedefines(out, thisone);
				}
			}
		}
		else out+=*here++;
	}
}

int repeatnext=1;

bool autocolon=false;

bool moreonline;
bool asarverallowed;
bool istoplevel;

extern int numtrue;
extern int numif;

extern int macrorecursion;

void assembleline(const char * fname, int linenum, const char * line)
{
	recurseblock rec;
	bool moreonlinetmp=moreonline;
	thisfilename=fname;
	thisline=linenum;
	thisblock=NULL;
	try
	{
		string tmp=line;
		if (!confirmquotes(tmp)) error<errline>(0, "Mismatched quotes");
		clean(tmp);
		string out;
		if (numif==numtrue) resolvedefines(out, tmp);
		else out=tmp;
		out.qreplace(": :", ":  :", true);
//puts(out);
		autoptr<char**> blocks=qsplit(out.str, " : ");
		moreonline=true;
		for (int block=0;moreonline;block++)
		{
			moreonline=blocks[block+1];
			int repeatthis=repeatnext;
			repeatnext=1;
			for (int i=0;i<repeatthis;i++)
			{
				try
				{
					itrim(blocks[block], " ", " ", true);
					thisfilename=fname;
					thisline=linenum;//do not optimize, this one is recursive
					thisblock=blocks[block];
					assembleblock(blocks[block]);
				}
				catch (errblock&) {}
				if (blocks[block][0]!='\0' && blocks[block][0]!='@') asarverallowed=false;
			}
		}
	}
	catch (errline&) {}
	moreonline=moreonlinetmp;
}

extern int numif;
extern int numtrue;

int incsrcdepth=0;

void assemblefile(const char * filename, bool toplevel)
{
	incsrcdepth++;
	thisfilename=filename;
	thisline=-1;
	thisblock=NULL;
	char ** lines;
	int startif=numif;
	if (!filecontents.find(filename, lines))
	{
		char * temp=readfile(filename);
		if (!temp)
		{
			error<errnull>(0, "Couldn't open file");
			return;
		}
		lines=split(temp, "\n");
		for (int i=0;lines[i];i++)
		{
			char * line=lines[i];
			char * comment=line;
			while ((comment=strqchr(comment, ';')))
			{
				if (comment[1]!='@') comment[0]='\0';
				else
				{
					if (strncasecmp(comment+2, "xkas", 4)) comment[1]=' ';
					comment[0]=' ';
				}
			}
			while (strqchr(line, '\t')) *strqchr(line, '\t')=' ';
			if (!confirmquotes(line)) { thisline=i; thisblock=line; error<errnull>(0, "Mismatched quotes"); line[0]='\0'; }
			itrim(line, " ", " ", true);
			for (int j=1;strqchr(line, ',') && !strqchr(line, ',')[1] && lines[i+j];j++)
			{
				strcat(line, lines[i+j]);
				static char nullstr[]="";
				lines[i+j]=nullstr;
			}
		}
		filecontents.insert(filename, lines);
	}
	bool inmacro=false;
	asarverallowed=true;
	for (int i=0;lines[i];i++)
	{
		try
		{
			thisfilename=filename;
			thisline=i;
			thisblock=NULL;
			istoplevel=toplevel;
			if (stribegin(lines[i], "macro ") && numif==numtrue)
			{
				if (inmacro) error<errline>(0, "Nested macro definition");
				inmacro=true;
				if (!pass) startmacro(lines[i]+6);
			}
			else if (!strcasecmp(lines[i], "endmacro") && numif==numtrue)
			{
				if (!inmacro) error<errline>(0, "Misplaced endmacro");
				inmacro=false;
				if (!pass) endmacro(true);
			}
			else if (inmacro)
			{
				if (!pass) tomacro(lines[i]);
			}
			else assembleline(filename, i, lines[i]);
		}
		catch (errline&) {}
		asarverallowed=false;
	}
	thisline++;
	thisblock=NULL;
	checkbankcross();
	if (inmacro)
	{
		error<errnull>(0, "Unclosed macro");
		if (!pass) endmacro(false);
	}
	if (repeatnext!=1)
	{
		repeatnext=1;
		error<errnull>(0, "rep at the end of a file");
	}
	if (numif!=startif)
	{
		numif=startif;
		numtrue=startif;
		error<errnull>(0, "Unclosed if statement");
	}
	incsrcdepth--;
}

bool checksum=true;
extern lightweight_map<string, unsigned int> labels;
extern autoarray<string> sublabels;
extern string ns;

struct macrodata
{
autoarray<string> lines;
int numlines;
int startline;
const char * fname;
const char ** arguments;
int numargs;
};
extern lightweight_map<string, macrodata*> macros;

#define cfree(x) free((void*)x)
static void clearmacro(const string & key, macrodata* & macro)
{
	macro->lines.~autoarray();
	cfree(macro->fname);
	cfree(macro->arguments[0]);
	cfree(macro->arguments);
	cfree(macro);
}

static void clearfile(const string & key, char** & filecontent)
{
	cfree(*filecontent);
	cfree(filecontent);
}

void reseteverything()
{
	string str;
	labels.clear();
	defines.clear();

	macros.traverse(clearmacro);
	macros.clear();
	//while (str=macros.rootKey())
	//{
	//	macrodata * macro;
	//	macros.find(str, macro);
	//	macro->lines.~autoarray();
	//	free(macro->fname);
	//	free(macro->arguments[0]);
	//	free(macro->arguments);
	//	free(macro);
	//	macros.remove(str);
	//}

	filecontents.traverse(clearfile);
	filecontents.clear();
	//while (str=filecontents.rootKey())
	//{
	//	char ** filecontent;
	//	filecontents.find(str, filecontent);
	//	free(*filecontent);
	//	free(filecontent);
	//	filecontents.remove(str);
	//}

	optimizeforbank=-1;
#undef free
}
