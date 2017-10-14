//include
//youdo
//end

enum Type(Line,Triangle,Rectangle,Circular,Ellipse,Arc,Curvebight2,Curvebight3);

class Graph{
	public:
		Type type;
		unsigned int shader_id;
		unsigned int shader_index;//用于保存，因为shader_id每次运行都可能不同，用来确定shader_id的值
		unsigned int VAO;
		unsigned int VEO;
		unsigned int VBO;

		void draw();
		
		void save_to_file(ofstream& out);
		void read_from_file(ifstream& in);
		void set_shader(int id,int index);
	private:
		//youdo
		//end
};


class Line:public Graph{
	public:
		Line();
		Line(const vec3 *ppoint,const vec4& color);

		~Line();
		void set_point(const vec3 *ppoint);
		void set_color(const vec4 &color);	

	private:
		vec3 point[2];
		vec3 color;
		//youdo
		//end
};

class Triangle:public Graph{
	public:

	private:
};

class Rectangle:public Graph{
	public:

	private:
};
class Circular:public Graph{
	public:

	private:
};
class Ellipse:public Graph{
	public:

	private:
};
class Arc:public Graph{
	public:

	private:
};

class  Curvebight2:public Graph{
	public:

	private:
};

class Curbight3:public Graph{
	public:

	private:

}
