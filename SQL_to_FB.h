#pragma once

#include "BaseSQL.h"
#include "Logger.h"

#include <exception>
#include "Windows.h"
#include <iostream>
#include <any>
#include <fstream>
#include <ctime> //либа длявывода текущего времени в лог

class SQL_to_FB : protected BaseSQL
{
public:
//	SQL_to_FB();
	SQL_to_FB(std::string webserver_, std::string path_, std::string user_, std::string pass_);
	~SQL_to_FB();

	bool connected();
	
    void get_param(std::string table, std::string coll_param, std::string coll_answer, std::any param, std::any &answer_);
    void get_param(std::string table, std::string coll_param, std::string coll_param_add, std::string coll_answer, std::any param, std::any param_add, std::any &answer_);
	
    void create_new_page(std::string name_page, int height, int width, std::string parentpage, std::string shablonpage, std::string FONCOLOR, std::string FRAMETYPE);
    int create_new_technical_programm(std::string name_page, int height, int width, std::string parentpage, std::string controller, std::string resource);
    int create_new_event(std::string name_event, std::string parent_event);
    void create_new_obj_onpage(std::string name_obj, int x, int y, int width, int height, std::string name_page,
		std::string type_obj, std::string obj, std::string FONCOLOR, std::string LINECOLOR, int group = 0, int idSubject = 0);
    void create_new_prim_onpage(std::string name_obj, std::string type_obj, std::string content, int x, int y, int width, int height, std::string name_page,
		int type, std::string FONCOLOR, std::string LINECOLOR, int group = 0);
	void create_new_anim_onpage(std::string type_anim, std::string content, std::string name_page, std::string name_obj, std::string parametr);
	void create_new_rec_onpage(std::string type_rec, std::string name_page, std::string name_obj, std::string parametr, std::string value);
	int object_on_technological_program(std::string name_page, std::string controller, std::string resource, std::string templateObj, std::string type_obj, std::string mark_obj);
	int create_new_object(std::string controller, std::string resource, std::string templateObj, std::string type_obj, std::string mark_obj, 
		std::string name_obj, std::string evklid_obj, std::string disc_obj, std::string KKS_obj, std::string sign_obj);
    int create_new_digital_object(std::string controller, std::string objectType, std::string mark, std::string name, std::string plcAdress,
		std::string plcVarname, std::string resource, std::string eventGroup, std::string description, std::string KKS, std::string signature);

	int copy_page_with_object(std::string templatePage, std::string namePage, std::string parentPage, std::string idObject, std::string eventGroup, std::string description = "");
	int create_new_driver(std::string mark, std::string nameDrive, std::string diskDrive, std::string signDrive, std::string typeDrive,
		std::string contorllDrive, std::string KKSDrive, std::string groupDrive);
	int presenceObj(std::string name, std::string typeObject, std::string controller = "", std::string resource = "");//проверка наличия объекта

private:

	
	int color_convet(std::string FONCOLOR);
	

    void get_type_prim(std::string type_obj, std::string content, int type, int &grobj_type, std::string &param_prim, int &penparams);
	void pars_type_prim(std::string content, std::string & txt, std::string & size, std::string & font, std::string & color);
    void get_type_anim(std::string type_anim, std::string parametr, std::string content, int &atribid, int &paramid, int &oper, int &constvalue, std::string &actparam, std::string &varerror);
	void get_type_rec(std::string type_rec, std::string parametr, std::string value, int & type_id, int & param_int, float & param_float, std::string & param_st, int & evparams, int & dparams, int & userright);
	void template_on_technological_program(std::any pageID, std::any templateID, int deltaX=0, int deltaY=0);
	void replaceString(std::string & input_string, const std::string searched_string, const std::string replace_string);
	void search_param(std::string inputString, std::string param, std::string &out, const char symbol);
	void moveTemplate(int deltaX, int deltaY, std::vector <std::any> list);

};

