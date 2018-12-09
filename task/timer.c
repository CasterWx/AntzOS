/* 定时器 */

#include <todo.h>

#define PIT_CTRL	0x0043
#define PIT_CNT0	0x0040

struct TIMERCTL timerctl;

void init_pit(void)
{
	io_out8(PIT_CTRL, 0x34);
	io_out8(PIT_CNT0, 0x9c);
	io_out8(PIT_CNT0, 0x2e);
	timerctl.count = 0;
	timerctl.flag = 0 ;
	return;
}


char * arr[] = {
	"    ",
	"    ",
  "    ",
  "    ",
  "    ",
  "    ",
  "    ",
  "    ",
  "    ",
  "    ",
	"    ",
	"    ",
	"    ",
	"    ",
	"    ",
	"    ",
	"package goddrinksjava;",
	"/**",
	"* The program GoddrinksJava implements",
	"* an application that creates an empty ",
	"* simulated world with no meaning or ",
	"* purpose.",
	"@ author = AntzUhl",
	"*/",
	"public class GodDrinksJava {",
	"     public static void main(String []args){",
	"         Thing me=new Lovable(Me,0,true,-1,false)",
	"         Thing you=new Lovable(You,0,false,-1,false)",
	"         World world=new World(5);",
	"         world.addThing(me);",
	"         world.addThing(you);",
	"    ",
	"    ",
	"         if(me instanceif PointSet)",
	"           you.addAttribute(me.getDimensions());",
	"           me.resetDimensions();",
	"         }",
	"         if(me instanceif Circle)",
	"           you.addAttribute(me.getCircumference());",
	"           me.resetCircumference();",
	"         }",
	"         if(me instanceif SineWave)",
	"           you.addAction(\"sit\",me.getTangent(you.XPosition))",
	"         }",
	"         if(me instanceif Sequence)",
	"           me.addAction(you.toLimit();",
	"         }",
	"    ",
	"    ",
	"         me.toggleCurrent();",
	"         me.canSee(false);",
	"         me.addFelling(\"dizzy\");",
	"         world.timeTravelForTwo(\"AD\",617,me.you);",
	"         world.timeTravelForTwo(\"BC\",3691,me.you);",
	"         world.unite(me,you);",
	"    ",
	"    ",
	"         if(me.getNumStimulationsAvailable() >=",
	"       you.getNumStimulationsNeeded()){",
	"          you.setSatisfaction(me.toSatisfaction());",
	"         }",
	"         if(you.getFeelingIndex(\"happy\")!=-1){",
	"          me.requestExecution(\"world\");",
	"         }",
	"         world.lockThing(me);",
	"         world.lockThing(you)",
	"    ",
	"    ",
	"         if(me instanceof Eggplant){",
	"            you.addAttribute(me.getNutrientts());",
	"            me.resetNutrients();",
	"         }",
	"         if(me instanceof Tomato){",
	"            you.addAttribute(me.getAntioxidants());",
 "            me.resetAntioxidants();",
 "         }",
 "         if(me instanceof TabbyCat){",
 "            me.purr();",
 "         }",
 "         if(world.getGod().equal(me)){",
 "            me.setProof(you.toProof());",
 "         }",
 "    ",
 "    ",
 "         me.toggleGender();",
 "    ",
 "         world.procreate(me,you);",
 "    ",
 "         me.toggleRoleBDSM();",
 "    ",
 "         world.makeHigh(me);",
 "    ",
 "         world.makeHigh(you);",
 "    ",
 "         if(me.getSenseIndex(\"vibration\")){",
 "            me.addFelling(\"complete\");",
 "         }",
 "    ",
 "         world.unlock(you);",
 "    ",
 "         world.removeThing(you);",
 "    ",
 "         me.lookFor(you,world);",
 "    ",
 "         me.lookFor(you,world);",
 "    ",
 "         me.lookFor(you,world);",
 "    ",
 "         me.lookFor(you,world);",
 "    ",
 "         me.lookFor(you,world);",
 "    ",
 "         me.lookFor(you,world);",
 "    ",
 "         me.lookFor(you,world);",
 "    ",
 "         if(me.getMemory().isErasable()){",
 "            me.removeFeeling(\"dishertened\");",
 "         }",
 "    ",
 "         try{",
 "            me.setOpinion(me.getOpinionIndex(\"you are here\"),false);",
 "         }",
 "         catch(IllegalArgumentException e){",
 "            world.announce(\"God is always true.\")",
 "         }",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.runExecution();",
 "    ",
 "         world.announce(\"1\",\"de\")",
 "         world.announce(\"2\",\"es\")",
 "         world.announce(\"3\",\"fr\")",
 "         world.announce(\"4\",\"kr\")",
 "         world.announce(\"5\",\"se\")",
 "         world.announce(\"6\",\"cn\")",
 "    ",
 "         world.runExecution();",
 "    ",
 "    ",
 "    ",
 "    ",
 "         if(world.isExecutableBy(me)){",
 "            you.setExecution(me.toExecution())",
 "         }",
 "    ",
 "         if(world.getThingIndex(you)!=-1){",
 "            world.runExecution()",
 "         }",
 "    ",
 "         me.escape(world)",
 "    ",
 "    ",
 "         me.learnTopic(\"love\")",
 "    ",
 "         me.takeExamTopic(\"love\")",
 "    ",
 "         me.getAlgebraicExpression(\"love\")",
 "    ",
 "         me.escape(\"love\")",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "         world.execute(me);",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
 "    ",
};
int i,j = 0 ;
int old_time = 0;
void init_data(void){
	i= 0;
	j= 0;
}

void inthandler20(int *esp)
{
	io_out8(PIC0_OCW2, 0x60); /* 把IRQ-00信号接收完了的信息通知给PIC */
	timerctl.count++;
	old_time ++ ;
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	// 绘制
	if((timerctl.flag==1)&&(old_time%10==0)){
		// 65
		print_area(binfo->vram, binfo->scrnx,COL8_000000,0, 0 ,binfo->scrnx,binfo->scrny);
		for (i=0;i<45;i++){
			print_string(binfo->vram, binfo->scrnx, binfo->scrnx/4-110,i*19+20, COL8_00FF00, arr[j+i]);
		}
		j += 1 ;
		if(j>=205){
			timerctl.flag = 0 ;
			init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);
			print_string(binfo->vram, binfo->scrnx,  0,  0, COL8_FFFFFF, "Terminal-Antz");
			print_string(binfo->vram, binfo->scrnx,  0,  0, COL8_000000, "Terminal-Antz");
			print_string(binfo->vram, binfo->scrnx,  binfo->scrnx/2-60,  0, COL8_000000, "|-|o|x|");
			print_string(binfo->vram, binfo->scrnx, 4, 19, COL8_FFFFFF, "AntzOS> SayHello()");
			print_string(binfo->vram, binfo->scrnx, 4, 38, COL8_FFFFFF, "Hello My AntzOs.");
			print_string(binfo->vram, binfo->scrnx, 4, 57, COL8_FFFFFF, "AntzOS>");
		}
	}
	return;
}
/*
int print_x = binfo->scrnx -70 ;
int print_y = binfo->scrny -40 ;

print_area(binfo->vram, binfo->scrnx,COL8_FFFFFF,print_x,print_y,print_x+60,print_y+35);

print_area(binfo->vram, binfo->scrnx,COL8_FFFFFF,print_x+10,print_y-20,print_x+20,print_y);
		print_area(binfo->vram, binfo->scrnx,COL8_008400,print_x+12,print_y-17,print_x+18,print_y);

print_area(binfo->vram, binfo->scrnx,COL8_FFFFFF,print_x+40,print_y-20,print_x+50,print_y);
		print_area(binfo->vram, binfo->scrnx,COL8_008400,print_x+42,print_y-17,print_x+48,print_y);


print_area(binfo->vram, binfo->scrnx,timerctl.count%15,print_x+13,print_y+13,print_x+18,print_y+22);
print_area(binfo->vram, binfo->scrnx,timerctl.count%15,print_x+43,print_y+13,print_x+48,print_y+22);

print_area(binfo->vram, binfo->scrnx,COL8_840000,print_x+22,print_y+25,print_x+40,print_y+30);

print_area(binfo->vram, binfo->scrnx,timerctl.count%15  ,print_x+27,print_y+25,print_x+30,print_y+27);
print_area(binfo->vram, binfo->scrnx,timerctl.count%14+1,print_x+30,print_y+25,print_x+33,print_y+27);
*/
