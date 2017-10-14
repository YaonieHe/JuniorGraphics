#ifndef SHADER_H
#define SHADER_H
//include
//youdo
//end

class Shader{
	public:
		unsigned int ID
		Shader(string vertex_shader_file,string framgment_shader_file);
		~Shader();
		void use();//using this Shader
		void set_bool(const string &name,bool vatue) const;
		void set_int(const string &name,int value) const;
		void set_float(const string &name,float value)const;
	private:
		
	//youdo
	//end
}


#endif
