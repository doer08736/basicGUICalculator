# include <ctype.h>
# include <string.h>
# include <gtk/gtk.h>
# include "./tools/stack.c"


char expr[30];
unsigned short int dot = 0;
unsigned short int flag = 0;


typedef struct{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button[19];
}calculator;


calculator widget;
GtkWidget *box;


void showExpr(){
    gtk_entry_set_text(GTK_ENTRY(box), expr);
}

void clearExpr(){
    dot = 0;
    flag = 0;
    memset(expr, 0, sizeof(expr));
    gtk_entry_set_text(GTK_ENTRY(box), "");
}

void backspace(){
    unsigned short int len;
    if((len=strlen(expr))){
        if(*(expr+(len-1))=='.')
            dot = 0;
        *(expr+(len-1)) = '\0';
    }
    showExpr();
}

unsigned short int checkStackSmash(){
    if(strlen(expr)>=30){
        gtk_entry_set_text(GTK_ENTRY(box), "Stack Smashed!! (Press C)");
        return 1;
    }return 0;
}

unsigned short int superWrong(){
    if(flag)
        return 1;
    return 0;
}

unsigned short int clearClicked(const gchar *text){
    return strcmp("C", text)==0;
}

unsigned short int backspaceClicked(const gchar *text){
    return strcmp("⌫", text)==0;
}

unsigned short int backToBackDot(const gchar *text){
    return *(text)=='.' && dot;
}

unsigned short int sameOperatorAgain(const gchar *text, char lastChar){
    return isOperator(*(text)) && lastChar==*(text);
}

unsigned short int invalidFirstInput(const gchar *text, char exprLen){
    return exprLen==0 && (
        *(text)=='+' ||
        *(text)=='=' ||
        *(text)=='/' || 
        *(text)=='x' || 
        *(text)=='%'
    );
}

unsigned short int invalidOperation(const gchar *text, char exprLen, char lastChar){
    return exprLen && (
        lastChar=='+' && *(text)=='-' ||
        lastChar=='+' && *(text)=='x' ||
        lastChar=='+' && *(text)=='/' ||
        lastChar=='+' && *(text)=='%' ||
        lastChar=='-' && *(text)=='+' ||
        lastChar=='-' && *(text)=='x' ||
        lastChar=='-' && *(text)=='/' ||
        lastChar=='-' && *(text)=='%' ||
        lastChar=='x' && *(text)=='+' || 
        lastChar=='x' && *(text)=='/' || 
        lastChar=='x' && *(text)=='%' || 
        lastChar=='/' && *(text)=='+' || 
        lastChar=='/' && *(text)=='x' || 
        lastChar=='/' && *(text)=='%' || 
        lastChar=='%' && *(text)=='+' || 
        lastChar=='%' && *(text)=='x' ||  
        lastChar=='%' && *(text)=='/' ||   
        lastChar=='%' && *(text)=='-'   
    );
}

unsigned short int validOperatorOrOperand(const gchar *text, char exprLen){
    return isdigit(*(text)) || 
        strcmp("%", text)==0 ||
        strcmp("/", text)==0 ||
        strcmp("x", text)==0 ||
        strcmp("-", text)==0 ||
        strcmp("+", text)==0 ||
        strcmp(".", text)==0;
}

void compute(){
    char str[30];
    evaluate(expr);
    sprintf(str, "%.2f", topOperand->data);
    gtk_entry_set_text(GTK_ENTRY(box), str);
    strncpy(expr, str, sizeof(expr));
}

void handleButtonClick(GtkButton *button, gpointer user_data){
    const gchar *text = gtk_button_get_label(button);

    char exprLen = strlen(expr);
    char lastChar = *(expr+(exprLen-1));

    if(clearClicked(text))
        clearExpr();

    else if(superWrong())
        return;

    else if(checkStackSmash())
        return;

    else if(invalidFirstInput(text, exprLen))
        return;

    else if(backToBackDot(text))
        return;
    
    else if(sameOperatorAgain(text, lastChar))
        return;

    else if(invalidOperation(text, exprLen, lastChar))
        return;

    else if(validOperatorOrOperand(text, exprLen)){
        if(*(text)=='.')
            dot = 1;
        if(isOperator(*(text)))
            dot = 0;
        strncat(expr, text, 30-exprLen);
        showExpr();
    }

    else if(backspaceClicked(text))
        backspace();

    else{
        if(isOperator(*(expr+(exprLen-1)))){
            flag = 1;
            gtk_entry_set_text(GTK_ENTRY(box), "super wrong exp!! (Press C)");
            return;
        }compute();
    }
}

void addButton(){
    const char *ops[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "C", "%", "⌫", "/", "x", "-", "+", "=", "."};
    for(register unsigned short int i=0; i<19; ++i)
        widget.button[i] = gtk_button_new_with_label(*(ops+i));
}

void attachButton(){
    gtk_grid_attach(GTK_GRID(widget.grid), box, 0, 0, 4, 1);
    gtk_grid_attach(GTK_GRID(widget.grid), widget.button[10], 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(widget.grid), widget.button[11], 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(widget.grid), widget.button[12], 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(widget.grid), widget.button[13], 3, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(widget.grid), widget.button[14], 3, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(widget.grid), widget.button[15], 3, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(widget.grid), widget.button[16], 3, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(widget.grid), widget.button[17], 3, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(widget.grid), widget.button[18], 2, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(widget.grid), widget.button[0], 0, 5, 2, 1);
    gtk_grid_attach(GTK_GRID(widget.grid), widget.button[1], 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(widget.grid), widget.button[2], 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(widget.grid), widget.button[3], 2, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(widget.grid), widget.button[4], 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(widget.grid), widget.button[5], 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(widget.grid), widget.button[6], 2, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(widget.grid), widget.button[7], 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(widget.grid), widget.button[8], 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(widget.grid), widget.button[9], 2, 2, 1, 1);
}

void connectClickedSignal(){
    for(register unsigned short int i=0; i<19; ++i)
        g_signal_connect(widget.button[i], "clicked", G_CALLBACK(handleButtonClick), NULL);
}

void activate(GtkApplication *app, gpointer user_data){

    widget.window = gtk_application_window_new(app);
    gtk_window_set_position(GTK_WINDOW(widget.window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(widget.window), "calculator");
    gtk_window_set_default_size(GTK_WINDOW(widget.window), 200, 200);
    gtk_container_set_border_width(GTK_CONTAINER(widget.window), 20);

    box = gtk_entry_new();
    widget.grid = gtk_grid_new();
    gtk_editable_set_editable(GTK_EDITABLE(box), TRUE);
    gtk_container_add(GTK_CONTAINER(widget.window), widget.grid);
    
    addButton();
    attachButton();
    connectClickedSignal();

    gtk_widget_show_all(widget.window);
}

int main(int argc, char **argv){
    int status;
    gtk_init(&argc, &argv);

    GtkApplication *app = gtk_application_new("mainstuff.gtk", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}