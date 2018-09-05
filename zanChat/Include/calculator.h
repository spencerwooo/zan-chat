#ifndef _CALCULATOR_H_
#define _CALCULATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

/**************************************************/
/*名称：		calculator.h
/*描述：		计算器小工具相关界面、接口
/***************************************************/

void calculatorInit();

void clearButtonClicked(GtkButton *button, gpointer data);

void numberButtonClicked(GtkButton *button, gpointer data);

void opreatorButtonClicked(GtkButton *button, gpointer data);

void dotButtonClicked(GtkButton *button, gpointer data);

void equalButtonClicked(GtkButton *button, gpointer data);

void createCalculator(GtkWidget *widget, gpointer data);

#endif
