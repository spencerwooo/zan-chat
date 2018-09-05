#include "./../../Include/calculator.h"

/**************************************************/
/*名称：		calculator.h
/*描述：		实现计算机插件的显示、输入、计算等相关功能
/*作成日期：	2017-08-31
/*作者：		唐回峰
/*
/**************************************************/
GtkWidget *entry;

gdouble num1, num2;
gboolean haveDot, haveResult, isFirst;
enum operator
{
    nothing,
    plus,
    minus,
    product,
    divide
} op;
gchar str[100];
gint cnt;
//entry、Gtkidget*、文本框用于显示结果
//num1,num2、gdouble、记录两操作数
//op、enum、记录操作符
//str、gchar[]、以记录字符串形式记录当前正在输入的操作数
//cnt、gint、限制操作数最大位数

/**************************************************/
/*名称：		calculator_init
/*描述：		初始化计算器状态
/*作成日期：	2017-08-31
/*参数：		VOID
/*返回值：	VOID
/*作者：		唐回峰
/***************************************************/
void calculatorInit()
{

    // HINT HERE
    // ZYC modified func name from 'init' 'to 'calculator_init'.

    memset(str, 0, sizeof(str));
    num1 = 0.0;
    num2 = 0.0;
    op = nothing;
    isFirst = TRUE;
    haveDot = FALSE;
    haveResult = FALSE;
    cnt = 0;
}

/**************************************************/
/*名称：		clearButtonClicked
/*描述：		重置计算器状态
/*作成日期：	2017-08-31
/*参数：		
		参数1：button、GtkButton、按下的按钮控件
        参数2：data、gpointer
/*返回值：	VOID
/*作者：		唐回峰
/***************************************************/
void clearButtonClicked(GtkButton *button, gpointer data)
{
    calculatorInit();
    gtk_entry_set_text(GTK_ENTRY(entry), "0");
}

/**************************************************/
/*名称：		numberButtonClicked
/*描述：		传递按下按钮对应的数字，
/*			经过判断后将其加入str，更新对应的操作数
/*作成日期：	2017-08-31
/*参数：		
		参数1：button、GtkButton、按下的按钮控件
        参数2：data、gpointer、指向整形的指针
/*返回值：	VOID
/*作者：		唐回峰
/***************************************************/
void numberButtonClicked(GtkButton *button, gpointer data)
{
    if (cnt == 9)
        return;
    if (haveResult)
        calculatorInit();

    cnt++;
    const gchar *tmp;
    tmp = gtk_button_get_label(GTK_BUTTON(button));
    gint pos = g_strlcat(str, tmp, 100);

    if (isFirst)
        num1 = strtod(str, NULL);
    else
    {
        num2 = strtod(str, NULL);
        // if(op == operator.divide && cnt == 1 && num2 == 0.0) return;
    }

    gtk_entry_set_text(GTK_ENTRY(entry), str);
}

/**************************************************/
/*名称：		opreatorButtonClicked
/*描述：		传递按下按钮对应的运算符，经过判断保存至op
/*作成日期：	2017-08-31
/*参数：		
		参数1：button、GtkButton、按下的按钮控件
        参数2：data、gpointer、指向char的指针
/*返回值：	VOID
/*作者：		唐回峰
/***************************************************/
void opreatorButtonClicked(GtkButton *button, gpointer data)
{
    switch (GPOINTER_TO_INT(data))
    {
    case 1:
        op = plus;
        break;
    case 2:
        op = minus;
        break;
    case 3:
        op = product;
        break;
    case 4:
        op = divide;
        break;
    }

    isFirst = FALSE;
    cnt = 0;
    haveDot = FALSE;
    memset(str, 0, sizeof(str));
}

/**************************************************/
/*名称：		dotButtonClicked
/*描述：		按下'.'按钮后，判断其作用
/*作成日期：	2017-08-31
/*参数：		
		参数1：button、GtkButton、按下的按钮控件
        参数2：data、gpointer、指向char的指针
/*返回值：	VOID
/*作者：		唐回峰
/***************************************************/
void dotButtonClicked(GtkButton *button, gpointer data)
{
    if (cnt == 0)
        gtk_entry_set_text(GTK_ENTRY(entry), "0.");
    else
    {
        if (haveDot == FALSE)
        {
            haveDot = TRUE;
            gint i = g_strlcat(str, ".", 100);
            if (isFirst)
                num1 = strtod(str, NULL);
            else
                num2 = strtod(str, NULL);

            gtk_entry_set_text(GTK_ENTRY(entry), str);
        }
    }
}

/**************************************************/
/*名称：		equalButtonClicked
/*描述：		按下'='按钮后，开始对 num1，num2，op进行运算
/*			将结果存入sum并显示
/*作成日期：	2017-08-31
/*参数：		
		参数1：button、GtkButton、按下的按钮控件
        参数2：data、gpointer、指向char的指针
/*返回值：	VOID
/*作者：		唐回峰
/***************************************************/
void equalButtonClicked(GtkButton *button, gpointer data)
{
    double ans = 0.0;
    gboolean dividedZero = FALSE, pressWithoutTwoNum = FALSE;
    char *res;
    char num[100];
    switch (op)
    {
    case plus:
        ans = num1 + num2;
        break;
    case minus:
        ans = num1 - num2;
        break;
    case product:
        ans = num1 * num2;
        break;
    case divide:
        if (num2 == 0.0)
        {
            dividedZero = TRUE;
            break;
        }
        ans = num1 / num2;
        break;
    default:
        gtk_entry_set_text(GTK_ENTRY(entry), str);
        pressWithoutTwoNum = TRUE;
        break;
    }

    if (pressWithoutTwoNum == FALSE)
    {
        if (dividedZero)
        {
            gtk_entry_set_text(GTK_ENTRY(entry), "Divided 0 !");
            calculatorInit();
            return;
        }
        res = g_ascii_dtostr(num, 100, ans);
        gtk_entry_set_text(GTK_ENTRY(entry), res);
        haveResult = TRUE;
        num1 = ans;
    }
}

/**************************************************/
/*名称：		createCalculator
/*描述：		显示计算器布局窗口
/*作成日期：	2017-08-31
/*参数：		
		参数1：button、GtkButton、按下的按钮控件
        参数2：data、gpointer
/*返回值：	VOID
/*作者：		唐回峰
/***************************************************/
void createCalculator(GtkWidget *widget, gpointer data)
{
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *hbox, *hbox1, *hbox2, *hbox3, *hbox4;
    GtkWidget *button;
    GtkWidget *label;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_widget_destroyed), NULL);

    calculatorInit();

    gtk_window_set_title(GTK_WINDOW(window), "计算器");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    entry = gtk_entry_new();
    gtk_widget_set_direction(entry, GTK_TEXT_DIR_RTL);
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 5);

    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 5);

    label = gtk_label_new("Calculator");
    gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 5);

    button = gtk_button_new_with_label("CE");
    gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 5);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(clearButtonClicked), NULL);

    hbox1 = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox1, FALSE, FALSE, 5);

    button = gtk_button_new_with_label("7");
    gtk_box_pack_start(GTK_BOX(hbox1), button, TRUE, TRUE, 5);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(numberButtonClicked), NULL);
    button = gtk_button_new_with_label("8");
    gtk_box_pack_start(GTK_BOX(hbox1), button, TRUE, TRUE, 5);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(numberButtonClicked), NULL);
    button = gtk_button_new_with_label("9");
    gtk_box_pack_start(GTK_BOX(hbox1), button, TRUE, TRUE, 5);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(numberButtonClicked), NULL);
    button = gtk_button_new_with_label("+");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(opreatorButtonClicked), (gpointer)1);
    gtk_box_pack_start(GTK_BOX(hbox1), button, TRUE, TRUE, 5);

    hbox2 = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox2, FALSE, FALSE, 5);

    button = gtk_button_new_with_label("4");
    gtk_box_pack_start(GTK_BOX(hbox2), button, TRUE, TRUE, 5);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(numberButtonClicked), NULL);
    button = gtk_button_new_with_label("5");
    gtk_box_pack_start(GTK_BOX(hbox2), button, TRUE, TRUE, 5);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(numberButtonClicked), NULL);
    button = gtk_button_new_with_label("6");
    gtk_box_pack_start(GTK_BOX(hbox2), button, TRUE, TRUE, 5);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(numberButtonClicked), NULL);
    button = gtk_button_new_with_label("-");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(opreatorButtonClicked), (gpointer)2);
    gtk_box_pack_start(GTK_BOX(hbox2), button, TRUE, TRUE, 5);

    hbox3 = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox3, FALSE, FALSE, 5);

    button = gtk_button_new_with_label("1");
    gtk_box_pack_start(GTK_BOX(hbox3), button, TRUE, TRUE, 5);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(numberButtonClicked), NULL);
    button = gtk_button_new_with_label("2");
    gtk_box_pack_start(GTK_BOX(hbox3), button, TRUE, TRUE, 5);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(numberButtonClicked), NULL);
    button = gtk_button_new_with_label("3");
    gtk_box_pack_start(GTK_BOX(hbox3), button, TRUE, TRUE, 5);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(numberButtonClicked), NULL);
    button = gtk_button_new_with_label("*");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(opreatorButtonClicked), (gpointer)3);
    gtk_box_pack_start(GTK_BOX(hbox3), button, TRUE, TRUE, 5);

    // button = gtk_button_new_with_label("/");
    // g_signal_connect(G_OBJECT(button),"clicked", G_CALLBACK(on_suan_clicked),(gpointer)4);
    // gtk_box_pack_start(GTK_BOX(hbox4),button,TRUE,TRUE,5);

    hbox4 = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox4, FALSE, FALSE, 5);
    button = gtk_button_new_with_label("0");
    gtk_box_pack_start(GTK_BOX(hbox4), button, TRUE, TRUE, 5);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(numberButtonClicked), NULL);
    button = gtk_button_new_with_label(".");
    gtk_box_pack_start(GTK_BOX(hbox4), button, TRUE, TRUE, 5);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(dotButtonClicked), NULL);
    button = gtk_button_new_with_label("=");
    gtk_box_pack_start(GTK_BOX(hbox4), button, TRUE, TRUE, 5);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(equalButtonClicked), NULL);
    button = gtk_button_new_with_label("/");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(opreatorButtonClicked), (gpointer)4);
    gtk_box_pack_start(GTK_BOX(hbox4), button, TRUE, TRUE, 5);

    gtk_widget_show_all(window);
}
