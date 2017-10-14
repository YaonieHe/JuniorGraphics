
//include
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

#include"graph_h.h"
#include"menu_h.h"
#include"shader_h.h"

//链表
template<class T>
class Link{
	public:
		T* obj;
		Link* next;
		Link* pre;
		Link():obj(NULL),next(NULL),link(NULL){};
};
namespace Pens{
	enum Pens_{select,fill,draw,deform,nodo}; 
};

namespace Deforms{
	enum Deforms_{translation,rotation,scaing,nodo};
}

//着色器个数
#define SHADER_NUM 5

GLFWwindow* window;//主窗口。
unsigned int SCR_WIDTH=800;//窗口宽
unsigned int SCR_HEIGHT=600;//窗口高

Link<Graph> *ALL_GRAPH;//所有图形，不包括当前没有画完的图形。(all graph expect havn't completed)
Link<Graph> *SELECTED_GRAPH;//所有被选择的图形。(all graph that be selected)
Link<Menu>  *ALL_MENU;//所有菜单项。
unsigned int ALL_SHADER[SHADER_NUM];//所有的填充模式

vec4 CURRENT_COLOR;//调色板当前颜色。
Pens CURRENT_PEN;//当前笔的类型。
unsigned int CURRENT_SHADER;//当前的填充模式，默认为白色填充。
Deforms CURRENT_DEFORM;//当前的变换模式。

Graph* CURRENT_GRAPH;//当前正在画的图形。
vec3 CURRENT_POINT;//鼠标当前所在位置。
unsigned int COMPLETED_POINT_NUM;//当前图形已画的点的个数。
vec3* COMPLETED_POINT;//当前图形的点集

string FILE_NAME;//文件名
unsigned int MODIFY_STATUS;//当前文件是否有修改。


void drawmenu();//画菜单
void drawpalette();//画图形
//窗口变化回调函数
void init();//初始化


void load_menus();//载入所有菜单项到ALL_MENU
void load_shaders();//载入所有作色器到ALL_SHADER
void update_cursor_location();//用CURRENT_POINT的值更新坐标菜单（显示鼠标所在位置）项；
void graph_deform(Graph*);//对图形做变形，即计算并改变图形的坐标位置。

Menu* get_cursor_location_menu();//获取鼠标所在位置的菜单。
Graph* get_cursor_location_graph();//获取鼠标所在位置的图形。

//各种菜单的点击响应函数：
//youdo
//end

void framebuffer_size_callback(GLFWwindow* window,int width,int height);
void key_callback(GLFWwindow* window,int key,int scancode,int action,int mods);
void cursor_position_callback(GLFWwindow* window,double xpos,double ypos);
void mouse_button_callback(GLFWwindow* window,int button,int action,int mods);



int main(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
#endif
	window=glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"Homework2",NULL,NULL);
	if(window == NULL){
		std::cout<<"Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrnt(window);

	//glad:load all OpenGL function pointers
	//-------------------------------------------
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout<<"Fail to initialize GLAD"<<std::endl;
		glfwTerminate();
		return -1;
	}
	
	
	
	//init: initialize all global variables and Menus
	init();
	
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
	glfwSetKeyBack(window,key_callback);
	glfwSetCursorPosCallback(window,cursor_pos_callback);
	glfwSetMouseButtonCallback(window,mouse_button_callback);

	show();
	


	glfwWaitEvent();

	return 0;
}


void init(){
	load_menus();
	load_shaders();
	ALL_GRAPH=NULL;
	SELECTED_GRAPH=NULL;
	
	CURRENT_PEN=PENS::nodo;
	CURRENT_GRAPH=NULL;
	CURRENT_COLOR=vec4(0.0f,0.0f,0.0f,1.0f);
	CURRENT_SHADER=ALL_SHADER[0];
	CURRENT_DEFORM=Deforms::nodo;

	CURRENT_POINT=vec3(0.0,0.0,0.0);
	COMPLETED_POINT_NUM=0;
	COMPLETED_POINT=NULL;
	
	FILE_NAME="";//临时文件名
	MODIFY_STATU=0；//刚载入，无修改。
}

void drawmenu(){
	//画所有的菜单
	Link<Menu>* pg=ALL_MENU;
	while(pg!=NULL){
		if(pg->obj!=NULL)
			pg->obj->draw();
		pg=pg->next;
	}
}

void drawpalette(){
	//画所有的图形
	Link<Graph>* pg=ALL_GRAPH;
	while(pg!=NULL){
		if(pg->obj!=NULL)
			pg->obj->draw();
		pg=pg->next;
	}
	//如果有正在画的图形，画出来。
	if(CURRENT_GRAPH!=NULL)
		CURRENT_GRAPH->draw();
	//如果有被选折的图形，再画一遍，让其显示在最上层。
	else if(SELECTED_GRAPH!=NULL){
		pg=SELECTED_GRAPH;
		while(pg!=NULL){
			if(pg->obj!=NULL)
				pg->obj->draw();
			pg=pg->next;
		}
	}
}


//显示、更新窗口
void show(){
	glViewport(0,0,SCR_WIDTH,SCR_HEIGHT);
	drawmenu();
//	float a[4];
//	a[0]*=SCR_WIDTH;a[1]*=SCR_HEIGHT;a[2]*=SCR_WIDTH;a[3]*=SCR_HEIGHT;
//	a[2]-=a[0];a[3]-=a[1];
//	glViewport((int)a[0],(int)a[1],(int)a[2],(int)a[3]);
	drawpalette();
	glfwSwapBuffers(window);
	glfwPollEvents();
	
}

//正常结束程序
void kill(){
	do_you_saved_file();
	glfwTerminate();
	exit(0);
}

//按键响应函数
void key_callback(GLFWwindow* window,int key,int scancode,int action,int mods){
	//按下esc，退出程序
	if(key==GLFW_KEY_ESCAPE && action==GLFW_PRESS)
		kill();
	//youdo
	//end
}

//鼠标移动响应函数
void cursor_position_callback(GLFWwindow* window,double xpos,double ypos){
	int state_left = glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT);
	
	float x=xpos,y=ypos;
	//更新当前点坐标
	CURRENT_POINT=vec3(x,y,0.0);
	update_cursor_location();

	//画笔状态
	if(CURRENT_PEN == Pens::draw){
		int n=CURRENT_GRAPH->get_point_num();//获取当前图形所需点的个数。
		for(int i=COMPLETED_POINT_NUM;i<n;i++){
			COMPLETED_POINT[i]=CURRENT_POINT;
		}
		//更新当前图形
		CURRENT_GRAPH->set_point(COMPLETED_POINT);
	}
	//变形
	else if(CURRENT_PEN == Pens::deform){
		if(state_left == GLFW_PRESS && COMPLETED_NUM==1){
			COMPLETED_POINT[1]=CURRENT_POINT;//completed_point[0]是鼠标按下时记录的坐标，1是鼠标移动后的坐标，变形处理时用这两个坐标计算图形结点的新值，然后set_point()。
			
			//对所有选中的图形做变形处理
			Link<Graph>* pg=SELECTED_GRAPHS;
			while(pg->obj!=NULL){
				graph_deform(pg->obj);
				pg=pg->next;
			}
		}
	}
	//其他情况不做处理。
	else{}
	//刷新
	show();
}

//鼠标点击响应函数
void mouse_button_callback(GLFWwindow* window,int button,int action,int mods){
	//鼠标左键按下

	//点击在关闭程序处
	if(glfwWindowShouldClose(window)){
		kill();
		return;
	}
	//当笔不是空笔时。
	if(button==GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		Menu* menu=get_cursor_location_menu();//获取鼠标所在位置的菜单
		//空白处，不处理。
		if(menu==NULL){
		
		}
		//点击在画板上
		else if(menu->type==Menu::palette){
			//对不同的笔分别处理
			if(CURRENT_PEN==Pens::select){
				//youdo
				//end
			}
			else if(CURRENT_PEN==Pens::draw){
				//youdo
				//end
			}
			else if(CURRENT_PEN==Pens::deform){
				//youdo
				//end
			}
			else{
			}
		}
		//其它菜单
		else{
			menu->button_func();//button_func是menu的一个函数指针对象，指向一个处理函数（在主程序中写，并在load_menus函数中赋给对应菜单）。
		}
	}
	
	//鼠标左键松开
	else if(button==GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
		//暂时不做处理，有需要再添加。
		//youdo
		//end
	}

	//其他情况不处理
	else{}
}

//窗口大小变化响应
void framebuffer_size_callback(GLFWwindow* window,int width,int heigh){
	SRC_WIDTH=width;
	SRC_HEIGHT=height;
	show();
}


//载入菜单
void load_menus(){
	
}

//载入着色器
void load_shaders(){

}

//用CURRENT_POINT的值更新坐标菜单（显示鼠标所在位置）项；
void update_cursor_location(){

}

//对图形做变形，即计算并改变图形的坐标位置。
void graph_deform(Graph*){

}

//获取鼠标所在位置的菜单。
Menu* get_cursor_location_menu(){

}

//获取鼠标所在位置的图形。
Graph* get_cursor_location_graph(){

}

//各种菜单的点击响应函数：
//youdo
//end
