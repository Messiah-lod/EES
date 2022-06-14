#include "SQL_to_FB.h"


//#pragma warning(disable : 4715)

#define BOOL_STR(b) (b?"true":"false")

SQL_to_FB::SQL_to_FB(std::string webserver_, std::string path_, std::string user_, std::string pass_)
    : BaseSQL ( webserver_, path_, user_, pass_)
{
    //вызван конструктор BaseSQL, с дальнейшим созданием подключения
}


SQL_to_FB::~SQL_to_FB()
{
    disconnect();
}

bool SQL_to_FB::connected()
{
    return BaseSQL::connected();
}

void SQL_to_FB::get_param(std::string table, std::string coll_param, std::string coll_answer, std::any param, std::any &answer_)
{
    query.clear();
    query = "Select " + coll_param + ", " + coll_answer + " from " + table + " where " + coll_param + " = '" + any_to_str(param) + "'";
    read(query, table, coll_answer, answer_);
}

void SQL_to_FB::get_param(std::string table, std::string coll_param, std::string coll_param_add, std::string coll_answer, std::any param, std::any param_add, std::any &answer_)
{
    query.clear();
    query = "Select " + coll_param + ", " + coll_param_add + ", " + coll_answer + " from " + table + " where " + coll_param + " = '" + any_to_str(param) + "' and " + coll_param_add + " = '" + any_to_str(param_add) + "'";
    read(query, table, coll_answer, answer_);
}


//void SQL_to_FB::print(std::vector <std::any> param)
//{
//	for (size_t i = 0; i < param.size(); i++)
//	{
//		logs << timeToLogs() << any_to_str(param[i]) << std::endl;
//	}
//}

void SQL_to_FB::create_new_page(std::string name_page, int height, int width, std::string parentpage, std::string shablonpage, std::string FONCOLOR, std::string FRAMETYPE)
{
    int color, FRAMENUM;
    std::any shablonpageID, PID, ID;
    color = 0;
    ID = PID = shablonpageID = 0;

    //перевод кодировки цвета в формат БД
    color = color_convet(FONCOLOR);
    //зададим тип открывания окна
    if (FRAMETYPE == "window")
    {
        FRAMENUM = 5;
    }
    else
    {
        FRAMENUM = 4;
    }

    //получим из БД  ID для parentpage
    get_param("GRPAGES", "NAME", "ID", parentpage, PID);
    get_param("GRPAGES", "NAME", "ID", shablonpage, shablonpageID);

    if (std::any_cast <int> (PID) == 0)
    {
        logs << "Родительский кадр найден! Создаем автоматически.";

        query.clear();
        query = "INSERT INTO GRPAGES(NAME,EDITBY ,HEIGHT, WIDTH, SHABLONPAGEID, FONCOLOR, PRINTWIDTH, PRINTHEIGHT, FRAMENUM) VALUES( '" +
                parentpage + "', " + "'SaprETO'" + ", " + std::to_string(height) + ", " + std::to_string(width) + ", " +
                std::to_string(0) + ", " + std::to_string(color) + ", " + std::to_string(height) + ", " + std::to_string(width) + ", " + std::to_string(FRAMENUM) + ")";
        write(query);

        get_param("GRPAGES", "NAME", "ID", parentpage, PID);
    }

    query.clear();
    query = "INSERT INTO GRPAGES(PID, NAME,EDITBY ,HEIGHT, WIDTH, SHABLONPAGEID, FONCOLOR, PRINTWIDTH, PRINTHEIGHT, FRAMENUM) VALUES(" +
            std::to_string(std::any_cast <int> (PID)) + ", '" + name_page + "', " + "'SaprETO'" + ", " + std::to_string(height) + ", " + std::to_string(width) + ", " +
            std::to_string(std::any_cast <int> (shablonpageID)) + ", " + std::to_string(color) + ", " + std::to_string(height) + ", " + std::to_string(width) + ", " + std::to_string(FRAMENUM) + ")";

    //выпоним запрос на запись
    write(query);
}

int SQL_to_FB::create_new_technical_programm(std::string name_page, int height, int width, std::string parentpage, std::string controller, std::string resource)
{
    std::vector <std::any> answ;
    std::any control, res, order, tmpOrder, tmpID, isaobjID, templateID, templateHeight, templateWidth, parentID, parentName;
    control = res = order = tmpOrder = tmpID = isaobjID = templateID = templateHeight = templateWidth = parentID = 0;
    bool errorName = false;
    bool errorNameTemplate = false;
    //находи ID переданного пользователем ресурса в указанном контроллере
    get_param("CARDS", "MARKA", "ID", controller, control);
    get_param("RESOURCES", "NAME", "CARDID", "ID", resource, control ,res);

    if (std::any_cast<int>(res) == 0)
    {
        logs << "Ошибка в методе create_new_technical_programm:";
        logs << "Ресурс или контроллер для создания тех. прграммы не найден!";
        return 4;
    }

    //	logs << "Имя запрошенной тех. программы: " << name_page << ",размер: " << std::to_string(name_page.size()) <<
    //	" Имя запрошенной группы тех. программ: " << parentpage << ",размер: " << std::to_string(parentpage.size()) << std::endl;

    //формируем запрос на получение ID всех техпрограмм, в запрашиваемом пользователем ресурсе
    query.clear();
    query = "SELECT ISAOBJID, RESOURCE FROM ISAPOUSTTEXT WHERE RESOURCE = '" + std::to_string(std::any_cast<int>(res)) + "'";
    read(query, "ISAPOUSTTEXT", "ISAOBJID", answ);

    //организуем цикл перебора всех тех. программа на поиск максимального номера исполнения тех. программы в ресурсе
    for (size_t i = 0; i < answ.size(); i++)
    {
        //logs << any_to_str(answ[i]) << std::endl;
        get_param("ISAOBJ", "ID", "\"ORDER\"", answ[i],  tmpOrder);
        //		get_param("ISAOBJ", "ID", "NAME", "ID", answ[i], name_page, tmpID);//поиск существующей порграммы по имени
        if (std::any_cast<int>(tmpOrder) > std::any_cast<int>(order))
        {
            order = tmpOrder;
        }
    }
    if (std::any_cast<int>(order) == 0 || std::any_cast<int>(order) == 1)
    {
        order = 3;//для обхода оглавления и титульного листа тех. программ
    }


    //Проверка корректности имени тех. прграммы
    errorName = !checkingIsCorrectName(name_page);
    //********Выполнять поиск ТОЛЬКО в найденном ресурсе!!!!*******
    get_param("ISAOBJ", "NAME", "ID", name_page, tmpID);
    if (std::any_cast<int>(tmpID) != 0) 	//прверим уникальность имени
    {
        logs << "Ошибка в методе create_new_technical_programm: имя не уникально! Создание новой программы не требуется.";
        logs << "Метод create_new_technical_programm: tmpID = " + QString::fromStdString(any_to_str(tmpID));
//        return std::any_cast<int>(tmpID);
        return 1;
    }


    //организуем цикл перебора всех тех. программ в необходимом ресурсе на поиск родителя
    for (size_t i = 0; i < answ.size(); i++)
    {
        get_param("ISAOBJ", "ID", "NAME", answ[i], parentName);//получаем имя тех.программы по ID
        if (any_to_str(parentName) == parentpage)
        {
            parentID = answ[i];
        }
    }
    //Если указанная группа тех. программы не найдена, создаем ее
    int count = 1;
    if (std::any_cast<int>(parentID) == 0)
    {
        errorNameTemplate = !checkingIsCorrectName(parentpage); //Проверка корректности имени группы тех. прграммы
        if (!errorNameTemplate)
        {
            logs << "Группа тех. программ не найдена. Создаем автоматически" ;
            //создадим группу тех. программ с +1 номером выполнения от максимального
            query.clear();
            query = "INSERT INTO ISAOBJ(NAME, KINDOBJ, USERID, GROUPID, LIBID, \"ORDER\") VALUES(  '" +
                    parentpage + "', 6, -1,-1 ,-1" + ", " + std::to_string(std::any_cast<int>(order) + count) + ")";
            count++;
            write(query);
            //Найдем максимальный ID
            query.clear();
            query = "select first 1 ID from ISAOBJ order by ID desc";
            read(query, "ISAOBJ", "ID", parentID);
            //запишем язык выполнения вновь созданной тех.программы 2 - ST, 5 - FBD из ISALANGUAGES
            query.clear();
            query = "INSERT INTO ISAPOU(ISAOBJID, LANGUAGE) VALUES(  " +
                    std::to_string(std::any_cast<int>(parentID)) + ", 2 ) ";
            write(query);
            //Присвоим нашей новой тех. программе ресурс
            query.clear();
            query = "UPDATE ISAPOUSTTEXT SET RESOURCE = '" + std::to_string(std::any_cast<int>(res)) +
                    "'  WHERE ISAOBJID = ' " + std::to_string(std::any_cast<int>(parentID)) + "' ";
            write(query);
        }
        else
        {
            logs << "Наименование группы тех. программ не корректно!";
            parentID = 0;
        }
    }

    if (!errorName){
        //создадим тех. программу с +1 (count, если пришлось создать группу тех. программ) номером выполнения от максимального
        query.clear();
        query = "INSERT INTO ISAOBJ(NAME, KINDOBJ, USERID, GROUPID, LIBID, \"ORDER\") VALUES(  '" +
                name_page + "', 6, -1,"+ any_to_str(parentID) +" ,-1" + ", " + std::to_string(std::any_cast<int>(order) + count) + ")";
        //выпоним запрос на запись
        write(query);

        //Найдем максимальный ID
        query.clear();
        query = "select first 1 ID from ISAOBJ order by ID desc";
        read(query, "ISAOBJ", "ID", isaobjID);
        //logs << any_to_str(isaobjID) << std::endl;

        //Настроим параметры тех. программы
        get_param("GRPAGES", "NAME", "ID", "Рамка A3 1:5", templateID);
        get_param("GRPAGES", "NAME", "HEIGHT", "Рамка A3 1:5", templateHeight);
        get_param("GRPAGES", "NAME", "WIDTH", "Рамка A3 1:5", templateWidth);
        //logs << std::to_string(std::any_cast<int>(templateHeight)) << "   " << std::to_string(std::any_cast<int>(templateWidth)) << std::endl;
        if (std::any_cast<int>(templateHeight) > height ||
                std::any_cast<int>(templateWidth) > width)
        {
            height = std::any_cast<int>(templateHeight);
            width = std::any_cast<int>(templateWidth);
        }
        query.clear();
        query = "INSERT INTO ISAGRPAGES(ID, EDITBY, HEIGHT , WIDTH, SHABLONPAGEID) VALUES(  " +
                std::to_string(std::any_cast<int>(isaobjID)) + ", 'SaprETO', " + std::to_string(height) +
                ", " + std::to_string(width) + ", " + std::to_string(std::any_cast<int>(templateID)) + ")";
        write(query);
        //запишем язык выполнения вновь созданной тех.программы 2 - ST, 5 - FBD из ISALANGUAGES
        query.clear();
        query = "INSERT INTO ISAPOU(ISAOBJID, LANGUAGE) VALUES(  " +
                std::to_string(std::any_cast<int>(isaobjID)) + ", 5 ) ";
        write(query);
        //Присвоим нашей новой тех. программе ресурс
        query.clear();
        query = "UPDATE ISAPOUSTTEXT SET RESOURCE = '" + std::to_string(std::any_cast<int>(res)) +
                "'  WHERE ISAOBJID = ' " + std::to_string(std::any_cast<int>(isaobjID)) + "' ";
        write(query);
        logs << "Новая тех. программа создана. Имя: " + QString::fromStdString(name_page) + ", ID: " + QString::fromStdString(any_to_str(isaobjID));
    }

    return std::any_cast<int>(isaobjID);
}


int SQL_to_FB::create_new_event(std::string name_event, std::string parent_event)
{
    logs.debug("Запрос на создание группы событий: " + QString::fromStdString(name_event));
    std::any PID = 0;
    //Проверим не пустое ли имя группы событий
    if (name_event.size() == 0)
    {
        logs << "Указанная группа событий имеет пустое имя. Группа событий создана не была.";
        return 2;
    }
    get_param("EVKLASSIFIKATOR", "NAME", "ID", name_event, PID);
    if (any_to_int(PID) != 0)
    {
        logs << "Указанная группа событий существует. ID = " + QString::fromStdString(any_to_str(PID)) + ". Создание новой не требуется.";
        return any_to_int(PID);
    }
    //Найдем родительскую группу событий
    PID = 0;
    get_param("EVKLASSIFIKATOR", "NAME", "ID", parent_event, PID);
    if (any_to_int(PID) == 0)
    {
        if (parent_event.size() == 0)
        {
            logs << "Указанная родительская группа событий имеет пустое имя.";
            PID = -2;
        }
        else
        {
            logs << "Родительская группа событий не найдена! Создаем автоматически: " + QString::fromStdString(parent_event);
            query.clear();
            query = "INSERT INTO EVKLASSIFIKATOR(PID, NAME, ARHCB)VALUES("
                    "-2 , '" + parent_event + "', 1)";
            write(query);
            get_param("EVKLASSIFIKATOR", "NAME", "ID", parent_event, PID);
        }
    }

    query.clear();
    query = "INSERT INTO EVKLASSIFIKATOR(PID, NAME, ARHCB)VALUES(" +
            any_to_str(PID) + ", '" + name_event + "', 1)";
    write(query);
    //Запишем ID созданной группы событий
    get_param("EVKLASSIFIKATOR", "NAME", "ID", name_event, PID);
    return any_to_int(PID);
}



void SQL_to_FB::create_new_obj_onpage(std::string name_obj, int x, int y, int width, int height, std::string name_page,
                                      std::string type_obj, std::string obj, std::string FONCOLOR, std::string LINECOLOR, int group, int idSubject)
{
    std::any page, IdTypeObj, IdObj, obj_width, obj_height;
    //	page = IdTypeObj = IdObj = 0;

    get_param("GRPAGES", "NAME", "ID", name_page, page);
    if (std::any_cast <int> (page) == 0)
    {
        logs << "Родительский кадр найден!";
    }

    //найдем ID библиотечного типа для размещения на кадре
    get_param("OBJTYPE", "NAME", "ID", type_obj, IdTypeObj);
    get_param("OBJMS", "NAME", "PID", "ID", obj, IdTypeObj, IdObj);

    if (width == 0 && height == 0)
    {
        //выгрузим из библиотеки размеры мнемосимвола
        get_param("OBJMSGRELEM", "PID", "GROBJTYPE", "WIDTH", IdObj, 9, obj_width);
        get_param("OBJMSGRELEM", "PID", "GROBJTYPE", "HEIGHT", IdObj, 9, obj_height);
    }
    else
    {
        obj_width = width;
        obj_height = height;
    }


    query.clear();
    query = "INSERT INTO PAGECONTENTS (GROBJTYPE, PAGEID, X, Y, WIDTH, HEIGHT, PARAMS, DRAWTYPE, PENCOLOR, BRUSHCOLOR, PENPARAMS, CARDID, OBJMSID, GRNUM, GRADCOLOR, NAME, GROUPID, LAYERNUM) VALUES(8," +
            std::to_string(std::any_cast <int> (page)) + ", " + std::to_string(x) + ", " +
            std::to_string(y) + ", " + std::to_string(std::any_cast <int> (obj_width)) +
            ", " + std::to_string(std::any_cast <int> (obj_height)) + ", '[MODE]=-1', 0, " +
            std::to_string(color_convet(LINECOLOR)) + ", " + std::to_string(color_convet(FONCOLOR)) + ", 1, " + std::to_string(idSubject) + "," +
            std::to_string(std::any_cast <int> (IdObj)) + ", " + std::to_string(group) + ", 0, '" + name_obj + "', 0, 1); ";

    //выпоним запрос на запись
    write(query);

}



void SQL_to_FB::create_new_prim_onpage(std::string name_obj, std::string type_obj, std::string content, int x, int y, int width, int height, std::string name_page,
                                       int type, std::string FONCOLOR, std::string LINECOLOR, int group)
{
    std::any page, IdTypeObj, IdObj, obj_width, obj_height;
    std::string param_prim;
    int penparams, obj_type = 0;

    //заполним тип и контент примитива
    get_type_prim(type_obj, content, type, obj_type, param_prim, penparams);

    //	std::cout << param_prim << " " << std::to_string(obj_type) << std::endl;

    //найдем id Родительского кадра
    get_param("GRPAGES", "NAME", "ID", name_page, page);
    if (std::any_cast <int> (page) == 0)
    {
        logs << "Родительский кадр найден!";
    }


    query.clear();
    query = "INSERT INTO PAGECONTENTS (GROBJTYPE, PAGEID, X, Y, WIDTH, HEIGHT, PARAMS, DRAWTYPE, PENCOLOR, BRUSHCOLOR, PENPARAMS, GRNUM, GRADCOLOR, NAME, GROUPID, LAYERNUM) VALUES(" +
            std::to_string(obj_type) + ", " + std::to_string(std::any_cast <int> (page)) + ", " + std::to_string(x) + ", " +
            std::to_string(y) + ", " + std::to_string(width) + ", " + std::to_string(height) + ", '" + param_prim + "', 0," +
            std::to_string(color_convet(LINECOLOR)) + ", " + std::to_string(color_convet(FONCOLOR)) + ", " + std::to_string(penparams) + "," + std::to_string(group) + ", 0, '" + name_obj + "', 0, 1);";
    //std::cout << query << std::endl;

    //выпоним запрос на запись
    write(query);
}

void SQL_to_FB::get_type_prim(std::string type_obj, std::string content, int type, int &grobj_type, std::string &param_prim, int &penparams)
{
    std::string txt, size, font, color;
    std::any penparam_any;


    if (type_obj == "text") //текстовый блок
    {
        pars_type_prim(content, txt, size, font, color);
        if (type == 0)
        {
            penparams = 337;
        }
        else
        {
            penparams = 1;
        }
        grobj_type = 1;
        param_prim = "[TEXT]=" + txt + "\r" +
                "[FONTID]=65535\r" +
                "[USERFONT]=" + size + ";" + font + ";0;" + std::to_string(color_convet(color)) + ";\r" +
                "[HINT]=\r [DELTAST]=2\r";
    }
    else if (type_obj == "rectangle") //прямоугольник
    {
        grobj_type = 2;
        param_prim = "[HINT]=";
    }
    else if (type_obj == "circle") //круг
    {
        grobj_type = 3;
        param_prim = "[HINT]=";
    }
    else if (type_obj == "arc") //дуга
    {
        grobj_type = 4;
        param_prim = "[POINT3]=100;20 [POINT4] = 0; 20 [HINT] =";
    }
    else if (type_obj == "image") //картинка
    {
        grobj_type = 5;
        param_prim = "[HINT]=\r";
        get_param("SYSVAR", "NAME", "ID", content, penparam_any);
        penparams = std::any_cast <int> (penparam_any);
    }
    else if (type_obj == "polygon") //многоугольник
    {
        grobj_type = 6;
        param_prim = "[PL]=(100,470);(160,470);(180,500);(140,520);(100,510);(100,480);(100,470); [HINT] =";
    }
    else if (type_obj == "polyline") //ломаная линия
    {
        grobj_type = 7;
        param_prim = "[PL]=(340,190);(440,200);(460,170);(530,170);	[HINT] =";
    }
    else if (type_obj == "spline") //сплайн
    {
        grobj_type = 10;
        param_prim = "[PL]=(340,240);(380,240);(410,280);(450,250);(490,240); [HINT] =";
    }
    else if (type_obj == "segment") //сегмент
    {
        grobj_type = 11;
        param_prim = "[POINT3]=100;20 [POINT4] = 0; 20 [HINT] =";
    }
    else if (type_obj == "sector")//сектор
    {
        grobj_type = 12;
        param_prim = "[POINT3]=50;0 [POINT4] = 0; 20 [HINT] =";
    }
    else if (type_obj == "rourectangle") //скругленный прямоугольник
    {
        grobj_type = 13;
        param_prim = "[POINT3]=20;0 [POINT4] = 0; 20 [HINT] =";
    }
    else if (type_obj == "button") //кнопка
    {
        pars_type_prim(content, txt, size, font, color);
        if (type == 0)
        {
            penparams = 337;
        }
        else
        {
            penparams = 1;
        }
        grobj_type = 14;
        //param_prim = "[TEXT]=Кнопка [FONTID] = 0 [USERFONT] = 8; Arial; 0; 0; [HINT] = [DELTAST] = 2 [PRESSCOLOR] = 536870911";
        param_prim = "[TEXT]=" + txt + "\r" +
                "[FONTID]=65535\r" +
                "[USERFONT]=" + size + ";" + font + ";0;" + std::to_string(color_convet(color)) + ";\r" +
                "[HINT]=\r [DELTAST]=2\r [PRESSCOLOR]=536870911\r";
    }
    else if (type_obj == "barograph") //барограф
    {
        grobj_type = 15;
        param_prim = "[HINT]=";
    }
    else if (type_obj == "vecdiag") //векторная диаграмма
    {
        grobj_type = 18;
        param_prim = "[USERFONT]=8;MS Sans Serif;0;-16777208; [VDEC] = 2 [SS] = 3 [LCFont] = 8; MS Sans Serif; 0; -16777208;[LVFont] = 8; MS Sans Serif; 0; -16777208;[LCColor] = 8421504[LVColor] = 16777215[LPenColor] = 0[NDColor] = 16777215";

    }
    else if (type_obj == "changehistory") //запись истории изменений
    {
        grobj_type = 19;
        param_prim = "[FONTID]=0 [USERFONT] = 8; Arial; 0; 0; [HINT] =";

    }
    else if (type_obj == "container") //контейнер
    {
        grobj_type = 25;
        param_prim = "";
    }
    else if (type_obj == "trend") //XY-тренд
    {
        grobj_type = 26;
        param_prim = "[USERFONT]=8;MS Sans Serif;0;-16777208;[AXISDECX] = 10[AXISDECY] = 10	[VALUEDEC] = 0 [VALUEDECY] = 0";

    }
    else if (type_obj == "checkbox") //чек-бокс
    {
        grobj_type = 27;
        param_prim = "[TEXT]=Опция [FONTID] = 0 [USERFONT] = 8; Arial; 0; 0; [HINT] = [DELTAST] = 2";
    }
    else if (type_obj == "radiobutton") //радио-баттон
    {
        grobj_type = 28;
        param_prim = "[TEXT]=Опция [FONTID] = 0 [USERFONT] = 8; Arial; 0; 0; [HINT] = [DELTAST] = 2";
    }
    else if (type_obj == "panel") //панель
    {
        grobj_type = 29;
        param_prim = "[LAYERS]=1:[по умолчанию]; [MARGIN] = 5 [MINHEIGHT] = 100";
    }
    else if (type_obj == "sellist") //список выбора
    {
        grobj_type = 50;
        param_prim = "[TEXT]=Вариант 1#Вариант 2#Вариант 3 [FONTID] = 0	[USERFONT] = 8; Arial; 0; 0;[HINT] = [DELTAST] = 10";
    }
    else if (type_obj == "scalebarograph") //шкала барографа
    {
        grobj_type = 51;
        param_prim = "[MAX]=100 [MIN] = 0 [MDC] = 10 [MDL] = 10 [MDW] = 2 [MDK] = 1 [SDC] = 5 [SDL] = 5 [SDW] = 1 [DEC] = 2 [BOF] = 2 [FONTID] = 0 [USERFONT] = 8; Arial; 0; 0; [HINT] = [YVA] = NAN [YVPA] = NAN [YVP] = NAN [YNA] = NAN [YNPA] = NAN [YNP] = NAN [YC1] = 536870911 [YC2] = 536870911 [YC3] = 536870911 [YC4] = 536870911 ";
    }
    else if (type_obj == "point") //точка перемещения
    {
        grobj_type = 52;
        param_prim = "[HINT]=";
    }
    else
    {
        logs << "Тип примитива не определен!";
        grobj_type = 0;
        param_prim = "";
    }
}

void SQL_to_FB::pars_type_prim(std::string content, std::string &txt, std::string &size, std::string &font, std::string &color)
{
    std::string tmp;
    int count = 0;

    count = 0;
    for (size_t i = 0; i < content.size(); i++)
    {
        tmp += content[i];
        if (content[i] == '[' || content[i] == ']')
        {
            if (count == 1)
            {
                txt = tmp;
            }
            else if (count == 3)
            {
                size = tmp;
            }
            else if (count == 5)
            {
                font = tmp;
            }
            else if (count == 7)
            {
                color = tmp;
            }
            count++;
            tmp.clear();
        }
    }
    try
    {
        txt.pop_back();
        size.pop_back();
        font.pop_back();
        color.pop_back();
    }
    catch (const std::exception&)
    {
        logs << "Параметры примитива не определены!";
    }
}

void SQL_to_FB::create_new_anim_onpage(std::string type_anim, std::string content, std::string name_page, std::string name_obj, std::string parametr)
{
    std::any id_page, id_prim;
    std::string varerror, actparam;
    int atribid, paramid, oper, constvalue;

    get_param("GRPAGES", "NAME", "ID", name_page, id_page);
    //	logs << timeToLogs() << any_to_str(id_page) << std::endl;
    if (std::any_cast <int> (id_page) == 0)
    {
        logs << "Кадр не найден!";
    }
    get_param("PAGECONTENTS", "PAGEID", "NAME", "ID", any_to_str(id_page), name_obj, id_prim);
    //	logs << timeToLogs() << any_to_str(id_prim) << std::endl;
    if (std::any_cast <int> (id_prim) == 0)
    {
        logs << "Родительский примитив для аниматора на кадре не найден!";
    }
    else
    {
        get_type_anim(type_anim, parametr, content, atribid, paramid, oper, constvalue, actparam, varerror);

        query.clear();
        query = "INSERT INTO PCANIMATOR (ATRIBID, PRIMID, PARAMID, OPER, CONSTVALUE, ACTPARAM, VARERROR) VALUES(" +
                std::to_string(atribid) + ", " + std::to_string(std::any_cast <int> (id_prim)) + ", " +
                std::to_string(paramid) + ", " + std::to_string(oper) + ", " + std::to_string(constvalue) + ", '" + actparam + "', " + varerror + ")";

        //std::cout << query << std::endl;
        //выпоним запрос на запись
        write(query);
    }
}

void SQL_to_FB::get_type_anim(std::string type_anim, std::string parametr, std::string content, int &atribid, int &paramid, int &oper, int &constvalue, std::string &actparam, std::string &varerror)
{
    std::any id_page, id_kat;
    std::string tmp, type_animimator, condition_anim, kat_anim, condition;
    //	int count = 0;
    atribid = 0;
    //разберем тип, переданного параметра, на составляющие
    pars_type_prim(type_anim, type_animimator, condition_anim, kat_anim, condition);
    //структура для определения типа аниматора
    if (type_animimator == "foncolor")
    {
        atribid = 2;
        actparam = std::to_string(color_convet(content));
    }
    else if (type_animimator == "flashfoncolor")
    {
        atribid = 4;
        tmp.clear();
        //Запишем первый параметр во временную строку
        for (size_t i = 0; i < content.size(); i++)
        {
            if (content[i] != ';')
            {
                tmp.push_back(content[i]);
            }
            else
            {
                break;
            }
        }
        //вырежем первый параметр из контента
        try
        {
            content.erase(0, tmp.size() + 1);
        }
        catch (std::out_of_range&)  //oops str is too short!!!
        {

        }
        actparam = tmp + ";" + std::to_string(color_convet(content));
        tmp.clear();
    }
    else if (type_animimator == "textcolor")
    {
        atribid = 7;
        actparam = std::to_string(color_convet(content));
    }
    else if (type_animimator == "visibility")
    {
        atribid = 1;
    }
    else if (type_animimator == "linecolor")
    {
        atribid = 3;
    }
    else if (type_animimator == "flashlinecolor")
    {
        atribid = 5;
    }
    else if (type_animimator == "flashvisibility")
    {
        atribid = 27;
    }
    else if (type_animimator == "textchange")
    {
        atribid = 6;
    }
    else if (type_animimator == "valueparam")
    {
        atribid = 8;
    }
    else if (type_animimator == "flashtext")
    {
        atribid = 9;
    }
    else if (type_animimator == "resreceptor")
    {
        atribid = 10;
    }
    else
    {
        logs << "Тип аниматора не определен!";
    }

    //структура для определения условия срабатывания аниматора
    if (condition_anim == "variable")
    {

    }
    else if (condition_anim == "event")
    {
        atribid += 100;
        get_param("EVKLASSIFIKATOR", "NAME", "ID", parametr, id_page);
        paramid = std::any_cast <int> (id_page);
        get_param("EVKAT", "NAME", "ID", kat_anim, id_kat);
        oper = std::any_cast <int> (id_kat);
        constvalue = std::stoi(condition);
        varerror = std::to_string(color_convet("223:220:215"));
    }
    else if (condition_anim == "eventchanel")
    {
        atribid += 600;
    }
    else if (condition_anim == "user")
    {
        atribid += 300;
    }
    else if (condition_anim == "page")
    {
        atribid += 500;
    }
    else if (condition_anim == "client")
    {
        atribid += 800;
    }
    else
    {
        logs << "Условие аниматора не определено!";
    }
}

void SQL_to_FB::create_new_rec_onpage(std::string type_rec, std::string name_page, std::string name_obj, std::string parametr, std::string value)
{
    std::any id_page, id_prim;
    std::string param_st;
    int type_id, param_int, evparams, dparams, userright;
    float param_float;

    get_param("GRPAGES", "NAME", "ID", name_page, id_page);
    if (std::any_cast <int> (id_page) == 0)
    {
        logs << "Родительский кадр не найден!";
    }
    get_param("PAGECONTENTS", "PAGEID", "NAME", "ID", any_to_str(id_page), name_obj, id_prim);
    if (std::any_cast <int> (id_prim) == 0)
    {
        logs << "Родительский примитив на кадре не найден!";
    }
    else
    {
        get_type_rec(type_rec, parametr, value, type_id, param_int, param_float, param_st, evparams, dparams, userright);

        query.clear();
        query = "INSERT INTO PCRECEPTOR (TYPEID, PRIMID, PARAM_INT, PARAM_FLOAT, PARAM_ST, EVPARAMS, DPARAMS, USERRIGHTS) VALUES(" +
                std::to_string(type_id) + ", " + std::to_string(std::any_cast <int> (id_prim)) + ", " +
                std::to_string(param_int) + ", " + std::to_string(param_float) + ", '" + param_st + "', " +
                std::to_string(evparams) + ", " + std::to_string(dparams) + ", " + std::to_string(userright) + ")";

        //std::cout << query << std::endl;
        //выпоним запрос на запись
        write(query);
    }
}

void SQL_to_FB::get_type_rec(std::string type_rec, std::string parametr, std::string value, int &type_id, int &param_int, float &param_float, std::string &param_st, int &evparams, int &dparams, int &userright)
{
    std::any id_event;
    std::string param_stst, evparamsst, dparamsst, userrightst;
    pars_type_prim(value, param_stst, evparamsst, dparamsst, userrightst);
    //структура для определения типа рецептора
    if (type_rec == "page")
    {
        type_id = 1;
    }
    else if (type_rec == "primpage")
    {
        type_id = 2;
    }
    else if (type_rec == "value")
    {
        type_id = 3;
    }
    else if (type_rec == "writeevent")
    {
        type_id = 4;
    }
    else if (type_rec == "opervalue")
    {
        type_id = 5;
    }
    else if (type_rec == "event")
    {
        type_id = 11;
        get_param("EVKLASSIFIKATOR", "NAME", "ID", parametr, id_event);
        param_int = std::any_cast <int> (id_event);
        param_float = 0;
        param_st = "FP:100;100;600;300;DP:" + param_stst + ";FL:();CP:";
        evparams = std::stoi(evparamsst);
        dparams = std::stoi(dparamsst);
        userright = std::stoi(userrightst);
    }
    else if (type_rec == "proframm")
    {
        type_id = 16;
    }
    else if (type_rec == "script")
    {
        type_id = 23;
    }
    else
    {
        logs << "Тип рецептора не определен!";
    }

}

int SQL_to_FB::object_on_technological_program(std::string name_page, std::string controller, std::string resource, std::string templateObj, std::string type_obj, std::string mark_obj)
{
    Q_UNUSED(templateObj)
    Q_UNUSED(type_obj)

    std::any control;//контроллер
    std::any res;//ресурс
    std::any templateID;//ID шаблона для тех. программы
    std::any pageID;//ID запрошенной тех. программы
    std::any objectID;//ID запрошенного пользователем объекта по марке в указанном ресурсе
    std::any tmpID;//ID для поиска привязанного объекта

    std::vector <std::any> objectIDlist;//лист для поиска ID требуемых привязки объектов
    std::vector <std::any> objectTemplateIDlist;//лист с шаблонами, которые будем менять
    //	std::vector <std::any> tmpListID;
    //листы для поиска требуемой тех. программы
    std::vector <std::any> pageIDlist;
    std::vector <std::any> pageIDResourceList;
    //лист для поиска блоков на шаблоне для привязки к объектам
    std::vector <std::any> blocksTemplateIDlist;

    templateID = pageID = objectID = tmpID = 0;

    //Найдем ID запрошенной тех. программы*****************************
    //находи ID переданного пользователем ресурса в указанном контроллере
    get_param("CARDS", "MARKA", "ID", controller, control);
    get_param("RESOURCES", "NAME", "CARDID", "ID", resource, control, res);
    //находим все ID с заданным пользователем именем тех.программы
    query.clear();
    query = "SELECT ID FROM ISAOBJ WHERE NAME = '" + name_page + " '";
    read(query, "ISAOBJ", "ID", pageIDlist);
    //находим все тех.программы, которые есть в запрошенном ресурсе
    //!!!можно ускорить переходом с циклов на запросы!!!
    query.clear();
    query = "SELECT ISAOBJID FROM ISAPOUSTTEXT WHERE RESOURCE = " + any_to_str(res);
    read(query, "ISAPOUSTTEXT", "ISAOBJID", pageIDResourceList);

    bool isBreak = false;
    for (size_t i = 0; i < pageIDlist.size(); i++)
    {
        for (size_t j = 0; j < pageIDResourceList.size(); j++)
        {
            if (std::any_cast <int> (pageIDlist[i]) == std::any_cast <int> (pageIDResourceList[j]))
            {
                pageID = pageIDlist[i];
                isBreak = true;
                break;
            }
        }
        if (isBreak)
        {
            break;
        }
    }
    if (std::any_cast <int> (pageID) == 0)
    {
        logs << "Исключение в методе object_on_technological_program:";
        logs << "ID технологической программы не найден! Выполнение метода закончилось с кодом 2.";
        return 2;
    }
    //ID техпрограммы найден*****************************************************

    //найдем ID требуемого шаблона*************************
    get_param("CARDS", "MARKA", "PLC_GR", "ID", mark_obj, any_to_str(res), objectID);
    if (std::any_cast <int> (objectID) == 0)
    {
        logs << "Исключение в методе object_on_technological_program:";
        logs << "ID объекта не найден! Выполнение метода закончилось с кодом 3.";
        return 3;
    }
    get_param("CARDS", "ID", "TEMPLATEID", any_to_str(objectID), templateID);
    if (std::any_cast <int> (templateID) == 0)
    {
        logs << "Исключение в методе object_on_technological_program:";
        logs << "ID шаблона не найден! Выполнение метода закончилось с кодом 1.";
        return 1;
    }
    //ID шаблона найден****************************

    template_on_technological_program(pageID, templateID);

    //***********************************************************************************************
    //***********************************************************************************************
    // 1. Определяем ID объектов, котоыре будем привязывать
    query.clear();
    query = "SELECT ID FROM ISACARDS WHERE CARDSID = " + any_to_str(objectID);
    read(query, "ISACARDS", "ID", objectIDlist);
    // 2. В ISAPAGECONTENTS организуем запрос на поиск по полю CARDID ID объектов, которые хотим привязать, если хоть 1 привязан уже где-то, метод прерываем
    for (size_t i = 0; i < objectIDlist.size(); i++)
    {
        get_param("ISAPAGECONTENTS", "CARDID", "CARDID", any_to_str(objectIDlist[i]), tmpID);
    }
    if (std::any_cast <int> (tmpID) != 0)
    {
        logs << "Исключение в методе object_on_technological_program:";
        logs << "Объект уже привязан! Выполнение метода закончилось с кодом 4.";
        return 4;
    }
    // 3. Определим в ISACARDS по листу ID объектов, котоыре будем привязывать, их шаблоны ISACARDSTEMPLATEID
    for (size_t i = 0; i < objectIDlist.size(); i++)
    {
        get_param("ISACARDS", "ID", "ISACARDSTEMPLATEID", any_to_str(objectIDlist[i]), tmpID);
        objectTemplateIDlist.push_back(tmpID);
    }
    // 4. Определим ID Объектов для привязки на ISAPAGECONTENTS запросом отбираем все объекты с GROBJTYPE = 31,37 и CARDID > 0, все эти объекты надо будет перепривязвывать
    query.clear();
    query = "SELECT ID FROM ISAPAGECONTENTS WHERE PAGEID is null AND (GROBJTYPE = 37 OR GROBJTYPE = 31) AND CARDID > 0";
    read(query, "ISAPAGECONTENTS", "ID", blocksTemplateIDlist);
    // 4. Берем лист ID для перепривязки, пробегаем по нему и заполняем в таблице ISAPAGECONTENTS у найденных ID объектов поле CARDID на ID, определенные в п.1
    for (size_t i = 0; i < blocksTemplateIDlist.size(); i++)
    {
        get_param("ISAPAGECONTENTS", "ID", "CARDID", any_to_str(blocksTemplateIDlist[i]), tmpID);

        for (size_t j = 0; j < objectTemplateIDlist.size(); j++)
        {
            if (std::any_cast <int> (objectTemplateIDlist[j]) == std::any_cast <int> (tmpID))
            {
                query.clear();
                query = "UPDATE ISAPAGECONTENTS SET CARDID = " + any_to_str(objectIDlist[j]) + " WHERE ID = " + any_to_str(blocksTemplateIDlist[i]);
                write(query);
                break;
            }
        }
    }
    //Поместим новые объекты на запрошенную тех. программу
    query.clear();
    query = "UPDATE ISAPAGECONTENTS SET PAGEID = " + any_to_str(pageID) +
            " WHERE PAGEID is null";
    write(query);
    //***********************************************************************************************
    //***********************************************************************************************


    //***********************************************************************************************
    //***********Привяжем блоки на тех. программе к типам объектов для генерации ST*************************
    // 1. Определяем ID объектов, котоыре будем привязывать
    objectIDlist.clear();
    query.clear();
    query = "SELECT ID FROM ISAPAGECONTENTS WHERE (FIELDSID <> 0) AND PAGEID = " + any_to_str(pageID);
    read(query, "ISAPAGECONTENTS", "ID", objectIDlist);
    // 2. Создадим необходимые переменные и идентифицируем их
    for (size_t i = 0; i < objectIDlist.size(); i++)
    {
        get_param("ISAPAGECONTENTS", "ID", "FIELDSID", any_to_str(objectIDlist[i]), tmpID);
        get_param("ISAPAGECONTENTS", "ID", "OBJMSID", any_to_str(objectIDlist[i]), objectID);

        //Создадим запрос на создание объекта
        query.clear();
        query = "INSERT INTO ISAOBJFIELDS (ISAOBJID, NAME, TID, DIRECTION)"
                "VALUES('" + any_to_str(pageID) + "', 'HV_" + any_to_str(objectIDlist[i]) + "', '" + any_to_str(objectID) + "', 4)";
        write(query);

        //Берем последний ID из таблицы
        query.clear();
        query = "SELECT ID FROM ISAOBJFIELDS WHERE id = (SELECT MAX(id) FROM ISAOBJFIELDS)";
        read(query, "ISAOBJFIELDS", "ID", tmpID);

        //обновляем новый ID в ISAPAGECONTENTS в поле FIELDSID
        query = "UPDATE ISAPAGECONTENTS SET FIELDSID = " + any_to_str(tmpID) +
                " WHERE ID = " + any_to_str(objectIDlist[i]);
        write(query);
    }
    /*
    После успешного создания объектов в таблице ISAPAGECONTENTS, необходимо отобрать все блоки, у которых FIELDSID <> 0
    сохранить их ID, FIELDSID, OBJMSID, PAGEID и создать в таблице ISAOBJFIELDS новые записи:
    ID = FIELDSID
    ISAOBJID = PAGEID  -   pageID
    NAME = "HV_" + ID
    TID = OBJMSID
    DIRECTION = 4

    Без данной операции компиляция тех. программ проходить не будет
    Заполнение SHORTNAME и DISC по желанию
    */

    return 0;
}

void SQL_to_FB::template_on_technological_program(std::any pageID, std::any templateID, int deltaX, int deltaY)
{
    std::any params;//параметр связи
    std::any fpParams;//параметр блока в связи
    std::any lpParams;//параметр блока в связи
    std::any fpID;//ID нового блока источника связи
    std::any lpID;//ID нового блока приемника связи
    std::any linkID;//ID новой связи

    std::string tmp, fp, lp;

    std::vector <std::any> linkIDlist;//ID всех связей в шаблоне
    //Листы для хранения ID новых блоков на заданной тех. программе
    std::vector <std::any> blocksIDlistProgram;
    //Листы для хранения ID существующих блоков на заданной тех. программе
    std::vector <std::any> oldBlocksIDlistProgram;
    //Листы для хранения параметра новых блоков на заданной тех. программе
    std::vector <std::any> blocksParamlistProgram;

    linkID = 0;

    if (std::any_cast <int> (templateID) != 0)
    {
        //Создадим запрос на копирование всех объектов требуемого шаблона
        query.clear();
        query = "INSERT INTO ISAPAGECONTENTS(GROBJTYPE, X, Y, WIDTH, HEIGHT, MSORT, PARAMS, DRAWTYPE, PENCOLOR, "
                "BRUSHCOLOR, PENPARAMS, CARDID, OBJMSID, GRNUM, GRADCOLOR, NAME, GROUPID, FIELDSID, LAYERNUM, TEMPLATEGROBJID)"
                "SELECT GROBJTYPE, X, Y, WIDTH, HEIGHT, MSORT, PARAMS, DRAWTYPE, PENCOLOR, BRUSHCOLOR, PENPARAMS, CARDID, OBJMSID, "
                "GRNUM, GRADCOLOR, NAME, GROUPID, FIELDSID, LAYERNUM, TEMPLATEGROBJID FROM ISAPAGECONTENTS WHERE PAGEID = '" +
                any_to_str(templateID) + "'";
        write(query);

        //запрос на лист всех ID связей с GROBJTYPE = 20 в нужном шаблоне, это связи блоков
        query.clear();
        query = "SELECT ID FROM ISAPAGECONTENTS WHERE PAGEID = " + any_to_str(templateID) + " AND GROBJTYPE = 20";
        read(query, "ISAPAGECONTENTS", "ID", linkIDlist);

        //Запросим все ID и параметры новых объектов
        query.clear();
        query = "SELECT ID FROM ISAPAGECONTENTS WHERE PAGEID is null";
        read(query, "ISAPAGECONTENTS", "ID", blocksIDlistProgram);
        query.clear();
        query = "SELECT PARAMS FROM ISAPAGECONTENTS WHERE PAGEID is null";
        read(query, "ISAPAGECONTENTS", "PARAMS", blocksParamlistProgram);

        //переберем все связи и заменим ID связуемых блоков
        for (size_t k = 0; k < linkIDlist.size(); k++)
        {
            tmp.clear();
            fp.clear();
            lp.clear();
            //по найденным ID получаем поле параметра с указанными ID связуемых блоков
            get_param("ISAPAGECONTENTS", "ID", "PARAMS", any_to_str(linkIDlist[k]), params);
            //Найдем ID этой же связи на новой тех. программе
            for (size_t i = 0; i < blocksParamlistProgram.size(); i++)
            {
                if (any_to_str(blocksParamlistProgram[i]) == any_to_str(params))
                {
                    linkID = blocksIDlistProgram[i];
                    break;
                }
            }
            tmp = any_to_str(params);
            //Парсим параметры связи и находим привязанные ранее ID Объектов
            search_param(tmp, "[FP]=", fp, '|');
            search_param(tmp, "[LP]=", lp, '|');
            //берем в параметрах связи блоков ID FP, LP на шаблоне, ищем у них параметр
            get_param("ISAPAGECONTENTS", "ID", "PAGEID", "PARAMS", fp, any_to_str(templateID), fpParams);
            get_param("ISAPAGECONTENTS", "ID", "PAGEID", "PARAMS", lp, any_to_str(templateID), lpParams);
            //по параметру на созданной техпрограмме ищем новые ID (FP, LP) новых блоков
            for (size_t i = 0; i < blocksParamlistProgram.size(); i++)
            {
                if (any_to_str(blocksParamlistProgram[i]) == any_to_str(fpParams))
                {
                    fpID = blocksIDlistProgram[i];
                    break;
                }
            }
            for (size_t i = 0; i < blocksParamlistProgram.size(); i++)
            {
                if (any_to_str(blocksParamlistProgram[i]) == any_to_str(lpParams))
                {
                    lpID = blocksIDlistProgram[i];
                    break;
                }
            }
            //вставляем новые ID в новую связь
            replaceString(tmp, fp, any_to_str(fpID));
            replaceString(tmp, lp, any_to_str(lpID));
            //формируем запрос на запись
            query.clear();
            query = "UPDATE ISAPAGECONTENTS SET PARAMS = '" + tmp +
                    "'  WHERE ID = ' " + any_to_str(linkID) + "' ";
            write(query);
        }
    }
    else
    {
        logs << "Исключение в методе template_on_technological_program:";
        logs << "Шаблон объекта не найден!";
        logs << "Создание объектов тех. программы выполнено не было.";
    }


    //*********************************************************************
    //*****Организуем сдвиг шаблона оносительно самой низкой точки текущих объектов
    int X = 0, Y = 0;//координаты начала установки шаблона
    int maxYPage = 0, maxHeightPage = 0, maxSummPage = 0;
    std::any tmpANY;

    query.clear();
    query = "SELECT ID FROM ISAPAGECONTENTS WHERE PAGEID = " + any_to_str(pageID);
    read(query, "ISAPAGECONTENTS", "ID", oldBlocksIDlistProgram);

    if (deltaX == 0 && deltaY == 0) {//если в вызове метода жестко прописали координаты сдвига
        for (size_t i = 0; i < oldBlocksIDlistProgram.size(); i++) {
            get_param("ISAPAGECONTENTS", "ID", "Y", any_to_str(oldBlocksIDlistProgram[i]), tmpANY);
            maxYPage = std::any_cast<int>(tmpANY);
            get_param("ISAPAGECONTENTS", "ID", "HEIGHT", any_to_str(oldBlocksIDlistProgram[i]), tmpANY);
            maxHeightPage = std::any_cast<int>(tmpANY);
            maxSummPage = maxYPage + maxHeightPage;
            if (Y < maxSummPage) Y = maxSummPage;
        }
        moveTemplate(X, Y, blocksIDlistProgram);
    }
    else moveTemplate(deltaX, deltaY, blocksIDlistProgram);


    //****************увеличим лист кратно шаблону
    for (size_t i = 0; i < blocksIDlistProgram.size(); i++)
    {
        oldBlocksIDlistProgram.push_back(blocksIDlistProgram[i]);
    }
    for (size_t i = 0; i < oldBlocksIDlistProgram.size(); i++) {
        get_param("ISAPAGECONTENTS", "ID", "Y", any_to_str(oldBlocksIDlistProgram[i]), tmpANY);
        maxYPage = std::any_cast<int>(tmpANY);
        get_param("ISAPAGECONTENTS", "ID", "HEIGHT", any_to_str(oldBlocksIDlistProgram[i]), tmpANY);
        maxHeightPage = std::any_cast<int>(tmpANY);
        maxSummPage = maxYPage + maxHeightPage;
        if (Y < maxSummPage) Y = maxSummPage;
    }
    int i = 1;
    while (Y > 1500 * i) i++;
    query = "UPDATE ISAGRPAGES SET HEIGHT = '" + std::to_string(1500*i) +
            "'  WHERE ID = ' " + any_to_str(pageID) + "' ";
    write(query);
}

void SQL_to_FB::replaceString(std::string& input_string, const std::string searched_string, const std::string replace_string)
{
    std::size_t replace_pos = input_string.find(searched_string);

    if (replace_pos != std::string::npos)
    {
        std::copy(std::begin(replace_string),
                  std::end(replace_string),
                  std::begin(input_string) + replace_pos
                  );

        //Or with a simple for loop (Which is what std::copy does):
        for (std::size_t i = 0; i < replace_string.size(); ++i)
            input_string[i + replace_pos] = replace_string[i];
    }
}

void SQL_to_FB::search_param(std::string inputString, std::string param, std::string & out, const char symbol)
{
    std::size_t replace_pos = inputString.find(param);

    if (replace_pos != std::string::npos)
    {
        for (size_t j = replace_pos + param.size(); j < inputString.size(); j++)
        {
            if (inputString[j] != symbol)
            {
                out += inputString[j];
            }
            else
            {
                break;
            }
        }
    }
}



int SQL_to_FB::create_new_object(std::string controller, std::string resource, std::string templateObj, std::string type_obj,
                                 std::string mark_obj, std::string name_obj, std::string evklid_obj, std::string disc_obj, std::string KKS_obj, std::string sign_obj)
{
    std::any control;//контроллер
    std::any res;//ресурс
    std::any templateID;//ID шаблона
    std::any objectID;//ID запрошенного пользователем объекта по марке в указанном ресурсе
    std::any evklID;//ID для группы событий

    std::any subjectID;//для возврата программы ID созданного запросом

    std::vector <std::any> objectIDlist;//лист для поиска ID объектов по типу
    std::vector <std::any> templateIDlist;//лист для поиска ID шаблонов объекта по имени
    std::vector <std::any> tmpIDlist;//временный лист для хранения ID шаблонов

    control = res = templateID = objectID = evklID = 0;

    //находи ID переданного пользователем ресурса в указанном контроллере
    get_param("CARDS", "MARKA", "ID", controller, control);
    get_param("RESOURCES", "NAME", "CARDID", "ID", resource, control, res);
    if (std::any_cast <int> (control) == 0 || std::any_cast <int> (res) == 0)
    {
        logs << "Исключение в методе create_new_object:";
        logs << "ID контроллера или ресурса не найден! Выполнение метода закончилось с кодом 1.";
        return 1;
    }

    //найдем ID требуемого шаблона и объекта*************************
    //находим все ID с заданным пользователем типом объекта
    query.clear();
    query = "SELECT ID FROM OBJTYPE WHERE NAME = '" + type_obj + " '";
    read(query, "OBJTYPE", "ID", objectIDlist);
    //	print(objectIDlist);
    //находим все ID с заданным пользователем именем шаблоном объекта
    query.clear();
    query = "SELECT ID FROM ISAOBJ WHERE NAME = '" + templateObj + " '";
    read(query, "ISAOBJ", "ID", templateIDlist);
    //	print(templateIDlist);

    bool isBreak = false;
    for (size_t i = 0; i < objectIDlist.size(); i++)
    {
        //находим все ID с шаблонами по найденному ID типу объекта и сравниваем ID в листе с ID с листом по имени
        query.clear();
        query = "SELECT ID FROM ISAGRPAGES WHERE OBJTYPEID = " + any_to_str(objectIDlist[i]);
        read(query, "ISAGRPAGES", "ID", tmpIDlist);
        for (size_t j = 0; j < tmpIDlist.size(); j++)
        {
            for (size_t k = 0; k < templateIDlist.size(); k++)
            {
                if (std::any_cast <int> (tmpIDlist[j]) == std::any_cast <int> (templateIDlist[k]))
                {
                    templateID = templateIDlist[k];
                    objectID = objectIDlist[i];
                    isBreak = true;
                    break;
                }
            }
            if (isBreak)
            {
                break;
            }
        }

        tmpIDlist.clear();
        if (isBreak)
        {
            break;
        }
    }
    //	logs << std::any_cast <int> (objectID) << "	" << std::any_cast <int> (templateID) << std::endl;

    if (std::any_cast <int> (templateID) == 0 || std::any_cast <int> (objectID) == 0)
    {
        logs << "Исключение в методе create_new_object:" ;
        logs << "ID объекта или шаблона не найден! Выполнение метода закончилось с кодом 2." ;
        return 2;
    }
    //ID шаблона и объекта найден****************************

    //Найдем группу событий объекта
    get_param("EVKLASSIFIKATOR", "NAME", "ID", evklid_obj, evklID);
    if (std::any_cast <int> (evklID) == 0)
    {
        logs << "Группа событий для объекта НЕ найдена. Объект помещен в группу [Все технологические]";
        evklID = -2;//группа все технологические
    }

    //находим все ID с заданным пользователем маркой объекта в найденном ресурсе
    tmpIDlist.clear();
    query.clear();
    query = "SELECT ID FROM CARDS WHERE PLC_GR = " + any_to_str(res) + " AND MARKA = '" + mark_obj + "'";
    read(query, "CARDS", "ID", tmpIDlist);
    if (tmpIDlist.size() != 0)
    {
        //		logs << "Исключение в методе create_new_object:";
        query.clear();
        query = "UPDATE CARDS SET NAME = '" + name_obj + "', DISC = '"+ disc_obj +"', OBJSIGN = '"+sign_obj+"', EVKLID = "+any_to_str(evklID)+
                " , KKS = '"+KKS_obj+"' WHERE ID = " + any_to_str(tmpIDlist[0]);
        write(query);

        logs << "Марка объекта уже существует! Поля объекта обновлены. Новый объект не создан. Метод вернул ID найденного объекта.";
 //       get_param("CARDS", "MARKA", "PLC_GR", "ID", mark_obj, any_to_str(res), subjectID);
        return 10;
    }

    //находим все ID с KKS объекта заданным пользователем
    if (KKS_obj.size() != 0 && KKS_obj.size() < 50)
    {
        tmpIDlist.clear();
        query.clear();
        query = "SELECT ID FROM CARDS WHERE KKS = '" + KKS_obj + "'";
        read(query, "CARDS", "ID", tmpIDlist);
        if (tmpIDlist.size() != 0)
        {
            logs << "Исключение в методе create_new_object:";
            logs << "KKS объекта уже существует! Объект не создан. Выполнение метода закончилось с кодом 3.";
            return 3;
        }
    }

    if (sign_obj.size() > 10)
    {
        sign_obj = "";
        logs << "Подпись объекта слишком длинная! Подпись стерта.";
    }

    //Создадим запрос на создание объекта
    query.clear();
    query = "INSERT INTO CARDS (MARKA, NAME, DISC, OBJSIGN, OBJTYPEID, PLC_GR, PLC_ID, ARH_PER, USERID, KLID, EVKLID, KKS, TEMPLATEID, LINKDEVICEID, SREZCONTROL)"
            "VALUES('" + mark_obj + "', '" + name_obj + "', '" + disc_obj + "', '" + sign_obj + "', " + any_to_str(objectID) + ", " + any_to_str(res) + ", " + any_to_str(control) +
            ", 0, -1, -2, " + any_to_str(evklID) + ", '" + KKS_obj + "', " + any_to_str(templateID) + ", 0, 1)";
    write(query);

    //как итог работы возвратим ID вновь созданного объекта
    get_param("CARDS", "MARKA", "PLC_GR", "ID", mark_obj, any_to_str(res), subjectID);
    return std::any_cast <int> (subjectID);
}

int SQL_to_FB::create_new_digital_object(std::string controller, std::string objectType, std::string mark, std::string name, std::string plcAdress,
                                         std::string plcVarname, std::string resource, std::string eventGroup, std::string description, std::string KKS, std::string signature)
{
    std::any control;//контроллер
    std::any templateID;//ID шаблона
    std::any objectID;//ID запрошенного пользователем объекта по марке в указанном ресурсе
    std::any evklID;//ID для группы событий

    std::any subjectID;//для возврата программы ID созданного запросом

    std::vector <std::any> tmpIDlist;//временный лист для хранения ID шаблонов

    control = templateID = objectID = evklID = 0;

    //находи ID переданного пользователем контроллера
    get_param("CARDS", "MARKA", "ID", controller, control);
    if (std::any_cast <int> (control) == 0)
    {
        logs << "Исключение в методе create_new_digital_object:";
        logs << "ID контроллера не найден! Выполнение метода закончилось с кодом 1.";
        return 1;
    }

    //находим ID с заданным пользователем типом объекта
    query.clear();
    query = "SELECT ID FROM OBJTYPE WHERE NAME = '" + objectType + "'";
    read(query, "OBJTYPE", "ID", objectID);
    if (std::any_cast <int> (objectID) == 0)
    {
        logs << "Исключение в методе create_new_digital_object:";
        logs << "ID объекта не найден! Выполнение метода закончилось с кодом 2.";
        return 2;
    }

    //Найдем группу событий объекта
    get_param("EVKLASSIFIKATOR", "NAME", "ID", eventGroup, evklID);
    if (std::any_cast <int> (evklID) == 0)
    {
        logs << "Группа событий для объекта НЕ найдена. Объект помещен в группу [Все технологические]";
        evklID = -2;//группа все технологические
    }

    //находим все ID с KKS объекта заданным пользователем
    if (KKS.size() != 0 && KKS.size() < 50)
    {
        tmpIDlist.clear();
        query.clear();
        query = "SELECT ID FROM CARDS WHERE KKS = '" + KKS + "'";
        read(query, "CARDS", "ID", tmpIDlist);
        if (tmpIDlist.size() != 0)
        {
            logs << "Исключение в методе create_new_digital_object: "
                    "KKS объекта уже существует! Объект не создан. Выполнение метода закончилось с кодом 3.";
            return 3;
        }
    }

    if (signature.size() > 10)
    {
        signature = "";
        logs << "Подпись объекта слишком длинная! Подпись стерта.";
    }

    get_param("CARDS", "MARKA", "PLC_ID", "ID", mark, any_to_str(control), subjectID);
 //   int tmpID = any_to_int(subjectID);//переменная для хранения найденного ID объекта по марке в контроллере
    bool errorMark = !checkingIsCorrectName(mark);
    if (errorMark)
    {
        logs << "Цифровой объект не был создан! Ошибка в марке либо марка не уникальна.";
        return 4;
    }
    if (any_to_int(subjectID) != 0){	//прверим уникальность имени
        query.clear();
        query = "UPDATE CARDS SET NAME = '" + name + "', DISC = '"+ description +"', OBJSIGN = '"+signature +"', EVKLID = "+any_to_str(evklID)+
                " , KKS = '"+KKS +"' WHERE ID = " + any_to_str(subjectID);
        write(query);
        logs << "Марка цифрового объекта не уникальна! Поля объекта были обновлены. Метод вернул ID обновленного объекта.";
        return 10;
    }

    if (resource.size() == 0) resource = "0";
    if (plcAdress.size() == 0) plcAdress = "0";
    //Создадим запрос на создание объекта
    query.clear();
    query = "INSERT INTO CARDS (MARKA, NAME, DISC, OBJSIGN, OBJTYPEID, PLC_GR, PLC_ADRESS, PLC_ID, PLC_VARNAME, ARH_PER, USERID, KLID, EVKLID, KKS, LINKDEVICEID, SREZCONTROL)"
            "VALUES('" + mark + "', '" + name + "', '" + description + "', '" + signature + "', " + any_to_str(objectID) + ", " + resource + ", " + plcAdress
            + ", " + any_to_str(control) + ", '" + plcVarname +  "', 0, -1, -2, " + any_to_str(evklID) + ", '" + KKS + "', 0, 1)";
    write(query);

    //как итог работы возвратим ID вновь созданного объекта
    get_param("CARDS", "MARKA", "PLC_ID", "ID", mark, any_to_str(control), subjectID);
    return std::any_cast <int> (subjectID);
}



int SQL_to_FB::copy_page_with_object(std::string templatePage, std::string namePage, std::string parentPage, std::string idObject, std::string eventGroup, std::string description)
{
    std::any idTemplatePage; //id шаблона копируемого объекта
    std::any pidPage; //id требуемого родителя
    std::any idNewPage; //id созданного кадра
    std::any idLibObj; //id типа объекта в библиотеке
    std::any tmpID;

    //******description - костыль для двойного подвязывания
    std::any idSecondObj; //id второго объекта для привязки на кадрах
    std::any idSecondLibObj; //id второго типа объекта для привязки на кадрах в библиотеке

    std::string nameTemplatePage; //имя шаблона копируемого объекта
    idNewPage = idTemplatePage = pidPage = tmpID = idLibObj = idSecondObj = idSecondLibObj = 0;

    std::vector <std::any> listIDObj; //Лист ID объектов на новом кадре
    std::vector <std::any> listIDTypeObj; //Лист ID типов объектов на новом кадре из библиотеки

    //Проверим не пустое ли имя кадра
    if (namePage.size() == 0)
    {
        logs << "Кадр имеет пустое имя! Кадр добавлен не был. Выход с кодом 3";
        return 3;
    }
    //Проверим нет ли уже такого кадра
    get_param("GRPAGES", "NAME", "ID", namePage, tmpID);
    if (std::any_cast <int> (tmpID) != 0)
    {
        logs << "Кадр с заданным именем уже существует! Кадр добавлен не был. Выход с кодом 1";
        return 1;
    }
    //Найдем ID кадра для копирования
    get_param("GRPAGES", "NAME", "ID", templatePage, idTemplatePage);
    if (std::any_cast <int> (idTemplatePage) == 0)
    {
        logs << "Шаблон для копирования кадра не найден! Кадр не был создан.";
        return 2;
    }

    //Создадим запрос на копирование кадра требуемого шаблона со всеми его параметрами кроме имени и родителя
    query.clear();
    query = "INSERT INTO GRPAGES (DPARAMS, EDITBY, HEIGHT, WIDTH, ZOOM, NUM, SHABLONPAGEID, FONCOLOR, X,  Y,"
            " FORMARKA, PRINTWIDTH, PRINTHEIGHT, PRINTPAGEA4, FRAMENUM, SREZ, SCRIPTCODE, DISC, LAYERS, HELPFILE)"
            "SELECT DPARAMS, EDITBY, HEIGHT, WIDTH, ZOOM, NUM, SHABLONPAGEID, FONCOLOR, X, Y, FORMARKA, "
            "PRINTWIDTH, PRINTHEIGHT, PRINTPAGEA4, FRAMENUM, SREZ, SCRIPTCODE, DISC, LAYERS, HELPFILE"
            " FROM GRPAGES WHERE ID =" + any_to_str(idTemplatePage);
    write(query);
    //Найдем ID созданного кадра и запишем в него требуемого родителя и имя кадра
    query.clear();
    query = "SELECT ID FROM GRPAGES WHERE NAME is null";
    read(query, "GRPAGES", "ID", idNewPage);

    get_param("GRPAGES", "NAME", "ID", parentPage, pidPage);
    if (std::any_cast <int> (pidPage) == 0)
    {
        logs << "Родительский кадр не найден! Создаем автоматически.";

        query.clear();
        query = "INSERT INTO GRPAGES(NAME,EDITBY ,HEIGHT, WIDTH, SHABLONPAGEID, FONCOLOR, PRINTWIDTH, PRINTHEIGHT, FRAMENUM) VALUES( '" +
                parentPage + "', " + "'SaprETO'" + ", " + std::to_string(1000) + ", " + std::to_string(1750) + ", " +
                std::to_string(0) + ", 7895160 , " + std::to_string(1750) + ", " + std::to_string(1000) + ", " + std::to_string(4) + ")";
        write(query);

        get_param("GRPAGES", "NAME", "ID", parentPage, pidPage);
    }

    query.clear();
    query = "UPDATE GRPAGES SET PID = " + any_to_str(pidPage) +
            " WHERE NAME is null";
    write(query);

    query.clear();
    query = "UPDATE GRPAGES SET NAME = '" + namePage +
            "' WHERE NAME is null";
    write(query);

    //копируем с шаблонного кадра все объекты и записываем новые ID  в лист для дальнейшей обработки
    query.clear();
    query = "INSERT INTO PAGECONTENTS (GROBJTYPE, X, Y, WIDTH, HEIGHT, MSORT, PARAMS, DRAWTYPE, PENCOLOR, BRUSHCOLOR,"
            " PENPARAMS, CARDID, OBJMSID, GRNUM, GRADCOLOR, NAME, GROUPID, LAYERNUM)"
            "SELECT GROBJTYPE, X, Y, WIDTH, HEIGHT, MSORT, PARAMS, DRAWTYPE, PENCOLOR, BRUSHCOLOR, PENPARAMS, "
            "CARDID, OBJMSID, GRNUM, GRADCOLOR, NAME, GROUPID, LAYERNUM"
            " FROM PAGECONTENTS WHERE PAGEID =" + any_to_str(idTemplatePage);
    write(query);

    query.clear();
    query = "SELECT ID FROM PAGECONTENTS WHERE PAGEID is null";
    read(query, "PAGECONTENTS", "ID", listIDObj);

    query.clear();
    query = "UPDATE PAGECONTENTS SET PAGEID = " + any_to_str(idNewPage) +
            " WHERE PAGEID is null";
    write(query);


    //******description - костыль для двойного подвязывания
    //Привяжем новые объекты к заданному
    //Получим id объекта, переданного нам экзампляра
    get_param("CARDS", "ID", "OBJTYPEID", idObject, idLibObj);

    //находи ID второго переданного пользователем объекта для привязки
    get_param("CARDS", "MARKA", "ID", description, idSecondObj);
    if (std::any_cast <int> (idSecondObj) == 0)
    {
        logs << "Исключение в методе copy_page_with_object:";
        logs << "ID второго объекта не найден!";
    }
    else
    {//Получим id объекта, переданного нам экзампляра
        get_param("CARDS", "ID", "OBJTYPEID", idSecondObj, idSecondLibObj);
    }

    //Получим лист соответствий id Объектов с их библиотечными типами
    for (size_t i = 0; i < listIDObj.size(); i++)
    {
        get_param("PAGECONTENTS", "ID", "OBJMSID", any_to_str(listIDObj[i]), tmpID);
        listIDTypeObj.push_back(tmpID);
    }

    //проверяем, если библиотечный тип принадлежит вновь создаваемому,привязываем их к нему
    for (size_t i = 0; i < listIDTypeObj.size(); i++)
    {
        get_param("OBJMS", "ID", "PID", any_to_str(listIDTypeObj[i]), tmpID);
        if (std::any_cast <int> (idLibObj) == std::any_cast <int> (tmpID))
        {
            //				logs << timeToLogs() << "Нашли первый тип" << std::endl;
            query.clear();
            query = "UPDATE PAGECONTENTS SET CARDID = " + idObject +
                    " WHERE ID = " + any_to_str(listIDObj[i]);
            write(query);
        }
        else if (std::any_cast <int> (idSecondLibObj) == std::any_cast <int> (tmpID))
        {
            //				logs << timeToLogs() << "Нашли второй тип" << std::endl;
            query.clear();
            query = "UPDATE PAGECONTENTS SET CARDID = " + any_to_str(idSecondObj) +
                    " WHERE ID = " + any_to_str(listIDObj[i]);
            write(query);
        }
    }



    //создадим аниматоры и рецепторы на новом кадре

    //проверить, на 2.8.2 какие-то изменения произошли*********************

    create_new_anim_onpage("[foncolor][event][Пред_2][1]", "0:128:255", namePage, "EVENT", eventGroup);
    create_new_anim_onpage("[foncolor][event][пред][1]", "255:255:0", namePage, "EVENT", eventGroup);
    create_new_anim_onpage("[foncolor][event][авар][1]", "255:0:0", namePage, "EVENT", eventGroup);

    create_new_anim_onpage("[flashfoncolor][event][Пред_2][0]", "0;128:128:128", namePage, "EVENT", eventGroup);
    create_new_anim_onpage("[flashfoncolor][event][пред][0]", "0;128:128:128", namePage, "EVENT", eventGroup);
    create_new_anim_onpage("[flashfoncolor][event][авар][0]", "0;128:128:128", namePage, "EVENT", eventGroup);

    create_new_anim_onpage("[textcolor][event][Пред_2][1]", "0:0:0", namePage, "EVENT", eventGroup);
    create_new_anim_onpage("[textcolor][event][пред][1]", "0:0:0", namePage, "EVENT", eventGroup);
    create_new_anim_onpage("[textcolor][event][авар][1]", "0:0:0", namePage, "EVENT", eventGroup);

    create_new_rec_onpage("event", namePage, "BUTEVENT", eventGroup, "[5][1][0][0]");

    return any_to_int(idNewPage);
}

int SQL_to_FB::create_new_driver(std::string mark, std::string nameDrive, std::string diskDrive, std::string signDrive, std::string typeDrive, std::string contorllDrive, std::string KKSDrive, std::string groupDrive)
{
    bool errorMark = false; //проверка валидности марки объекта

    std::any objectID; //ID Типа объекта
    std::any serverID; //ID сервера райвера
    std::any groupID; //ID объединения
    std::any runTypeID; //ID выполнения драйвера (локальный/внутреннй)
    std::any tmpID; //ID для проверки уникальности марки драйвера

    objectID = serverID = groupID = runTypeID = 0;

    get_param("CARDS", "MARKA", "ID", contorllDrive,serverID);
    get_param("CARDS", "MARKA", "ID", groupDrive, groupID);
    get_param("OBJTYPE", "NAME", "ID", typeDrive, objectID);
    get_param("CARDS", "MARKA", "ID", mark, tmpID);

    errorMark = !checkingIsCorrectName(mark);
    if (errorMark)
    {
        logs << "Драйвер не был создан! Ошибка в марке либо марка не уникальна.";
//        return std::any_cast<int>(tmpID);
        return 3;
    }
    if (any_to_int(tmpID) != 0){	//прверим уникальность имени
        query.clear();
        query = "UPDATE CARDS SET NAME = '" + nameDrive + "', DISC = '"+ diskDrive +
                "' , KKS = '"+KKSDrive +"' WHERE ID = " + any_to_str(tmpID);
        write(query);
        logs << "Марка драйвера не уникальна! Поля объекта были обновлены. Метод вернул ID обновленного драйвера.";
        return 10;
    }

    if (std::any_cast <int> (serverID) == 0)
    {
        logs << "Драйвер не был создан! Ошибка в определении провайдера.";
        return 1;
    }
    if (std::any_cast <int> (objectID) == 0)
    {
        logs << "Драйвер не был создан! Ошибка в определении типа драйвера.";
        return 4;
    }

    if (signDrive == "Внутренний")
    {
        get_param("CARDS", "MARKA", "PLC_ID", "ID", "[Внутренние драйвера шлюза]", any_to_str(serverID), runTypeID);
    }
    else
    {
        get_param("CARDS", "MARKA", "PLC_ID", "ID", "[Локальные драйвера шлюза]", any_to_str(serverID), runTypeID);
    }

    if (std::any_cast <int> (runTypeID) == 0)
    {
        logs << "Драйвер не был создан! Ошибка в определении типа исполнения драйвера (локальный/внутренний).";
        return 2;
    }

    //Создадим запрос на создание объекта
    query.clear();
    query = "INSERT INTO CARDS (MARKA, NAME, DISC, OBJTYPEID, PLC_GR, PLC_ADRESS, PLC_ID, ARH_PER, USERID, KLID, EVKLID, KKS, TEMPLATEID, LINKDEVICEID, SREZCONTROL)"
            "VALUES('" + mark + "', '" + nameDrive + "', '" + diskDrive + "', " + any_to_str(objectID) + ", " + any_to_str(groupID) + ", " + any_to_str(runTypeID) + ", " + any_to_str(serverID) +
            ", 0, -1, -7, -100 , '" + KKSDrive + "', 0, 0, 1)";
    write(query);

    //как итог работы возвратим ID вновь созданного объекта
    get_param("CARDS", "MARKA", "ID", mark, objectID);
    return std::any_cast <int> (objectID);
}

int SQL_to_FB::presenceObj(std::string name, std::string typeObject, std::string controller, std::string resource)
{
    std::any objectID; //ID объекта
    std::any control;//контроллер
    std::any res;//ресурс

    control = res = objectID = 0;

    if (typeObject == "object")
    {
        //находи ID переданного пользователем ресурса в указанном контроллере
        get_param("CARDS", "MARKA", "ID", controller, control);
        get_param("RESOURCES", "NAME", "CARDID", "ID", resource, control, res);
        if (std::any_cast <int> (control) == 0 || std::any_cast <int> (res) == 0)
        {
            logs << "Исключение в методе presenceObj:";
            logs << "ID контроллера или ресурса не найден! Выполнение метода закончилось с кодом 1.";
            return 1;
        }
        //как итог работы возвратим ID объекта
        get_param("CARDS", "MARKA", "PLC_GR", "ID", name, any_to_str(res), objectID);
        logs << "Метод presenceObj: objectID = " + QString::fromStdString(any_to_str(objectID));
        return std::any_cast <int> (objectID);
    }
    else if (typeObject == "mnemonic")
    {
        //Найдем ID кадра
        get_param("GRPAGES", "NAME", "ID", name, objectID);
        logs << "Метод presenceObj: objectID = " + QString::fromStdString(any_to_str(objectID));
        return std::any_cast <int> (objectID);
    }
    else if (typeObject == "programm")
    {
        //как итог работы возвратим ID объекта
        get_param("CARDS", "MARKA", "ID", name, objectID);
        logs << "Метод presenceObj: objectID = " + QString::fromStdString(any_to_str(objectID));
        return std::any_cast <int> (objectID);
    }
    else
    {
        logs << "Исключение в методе presenceObj.";
        logs << "Не верно указан тип объекта! ID не был возвращен.";
        return 1; //если
    }
}

void SQL_to_FB::moveTemplate(int deltaX, int deltaY, std::vector <std::any> listIdObj)
{
    const int DELTA = 100;//расстояние от последнего блока до нового
    std::any X, Y;
    std::any params;
    std::string parametrSTR, tmp;
    std::string coordX;
    std::string coordY;
    unsigned int count = 0;
    unsigned int nposParams = 0; //номер позиции параметра PL
    unsigned int sizeParams = 0;

    for (size_t i = 0; i < listIdObj.size(); i++) {
        X = 0; Y = 0; sizeParams = 0; parametrSTR.clear();
        count = 0;
        get_param("ISAPAGECONTENTS", "ID", "Y", listIdObj[i], Y);
        get_param("ISAPAGECONTENTS", "ID", "X", listIdObj[i], X);

        //		logs << timeToLogs() << "Стартовые координаты X = " << any_to_str(X) << "  и Y = " << any_to_str(Y) << std::endl;

        get_param("ISAPAGECONTENTS", "ID", "GROBJTYPE", "PARAMS" ,listIdObj[i], "20", params);

        if (any_to_str(params).size() > 1) {
            search_param(any_to_str(params), "[PL]=", parametrSTR, '[');
            tmp = any_to_str(params);
            nposParams = tmp.find(parametrSTR);//сохраним позицию для вставки нового параметра
            sizeParams = parametrSTR.size() - 2;
            tmp.erase(nposParams, sizeParams);

            //			logs << timeToLogs() << "Стартовые параметры полилинии = " << parametrSTR << std::endl;

            while (count < parametrSTR.size())
            {
                coordX.clear();
                coordY.clear();
                //блок для Х
                if (parametrSTR[count] == '(') {
                    count++;
                    while (parametrSTR[count] != ',') {
                        coordX += parametrSTR[count];
                        parametrSTR.erase(count, 1);
                    }
                    parametrSTR.insert(count, std::to_string(stoi(coordX) + DELTA + deltaX));
                }
                //блок для Y
                if (parametrSTR[count] == ',') {
                    count++;
                    while (parametrSTR[count] != ')') {
                        coordY += parametrSTR[count];
                        parametrSTR.erase(count, 1);
                    }
                    parametrSTR.insert(count, std::to_string(stoi(coordY) + DELTA + deltaY));
                }

                count++;

            }
            tmp.insert(nposParams, parametrSTR);

            //			logs << timeToLogs() << "Измененные параметры полилинии = " << parametrSTR << std::endl;

            query = "UPDATE ISAPAGECONTENTS SET PARAMS = '" + tmp +
                    "'  WHERE ID = ' " + any_to_str(listIdObj[i]) + "' ";
            write(query);
        }

        X = std::any_cast<int>(X) + DELTA + deltaX;
        Y = std::any_cast<int>(Y) + DELTA + deltaY;

        //		logs << timeToLogs() << "Измененные координаты X = " << any_to_str(X) << "  и Y = " << any_to_str(Y) << std::endl;

        query = "UPDATE ISAPAGECONTENTS SET X = " + any_to_str(X) +
                "  WHERE ID = ' " + any_to_str(listIdObj[i]) + "' ";
        write(query);
        query = "UPDATE ISAPAGECONTENTS SET Y = " + any_to_str(Y) +
                "  WHERE ID = ' " + any_to_str(listIdObj[i]) + "' ";
        write(query);
    }
}
