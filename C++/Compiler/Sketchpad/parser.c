#include "parser.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <assert.h>

BOOL in_image_parser = FALSE;
int cur_line = 0; // current processing line
FILE * fp = NULL;
FILE * ske = NULL;
char * cur_strline = NULL;
char * str_insrructions[] = { "Image", "lineTo", "End", "print", "draw", "translate", "child", "rotate" };
ImageList il = { NULL, NULL };
char * COMMAND =  "java -jar Sketchpad.jar ";

//static void clean();
//static exit_and_clean();
//static BOOL is_image_instr(char * str);
//static BOOL is_print_instr(char * str);
//static BOOL is_draw_instr(char * str);
//static BOOL is_translate_instr(char * str);
//static BOOL is_rotate_instr(char * str);
//static BOOL is_child_instr(char * str);
//static BOOL is_lineto_instr(char * str);
//static BOOL is_end_instr(char * str);
//static BOOL is_toplevel_instr(char *str, int *cmd_type);
//static char * lstrip(char * pstr);
//static char * copy_str(const char * str);
//static char * parser_command(const char * ptr, char ** out);
//static char * pasrer_image_name(char * ptr, char ** out);
//static char * parser_number(char * ptr, double* num);
//static void skip_space(const char * ptr);
//static void image_add_point(ImageInfo * pii, Point ppt);
//static char * filter_blankline_and_comment(FILE * fp);

static void clean_point_list(PointList * lst)
{
	PointList * tmp;
	while(lst){
		tmp = lst;
		lst = lst->next;
		free(tmp);
	}
}

static void clean()
{
	ImageList *pil,*tmp;

	if(fp){
		fclose(fp);
		fp = NULL;
	}
	if(ske){
		pclose(ske);
		ske = NULL;
	}

	pil = &il;
	if(pil->img == NULL)return;
	while(pil){
		tmp = pil;
		clean_point_list(pil->img->lst);
		pil = pil->next;
		if(tmp != &il){
			free(tmp);
		}
	}
	il.img = il.next = NULL;
}	

static void exit_and_clean()
{
	error(cur_line);
	clean();
	exit(EXIT_FAILURE);
}


static BOOL is_image_instr(char * str)
{
	return strcmp(str, "Image") == 0;
}

static BOOL is_print_instr(char * str)
{
	return strcmp(str, "print") == 0;
}

static BOOL is_draw_instr(char * str)
{
	return strcmp(str, "draw") == 0;
}

static BOOL is_translate_instr(char * str)
{
	return strcmp(str, "translate") == 0;
}

static BOOL is_rotate_instr(char * str)
{
	return strcmp(str, "rotate") == 0;
}

static BOOL is_child_instr(char * str)
{
	return strcmp(str, "child") == 0;
}

static BOOL is_lineto_instr(char * str)
{
	return strcmp(str, "lineTo") == 0;
}

static BOOL is_end_instr(char * str)
{
	return strcmp(str, "End") == 0;
}

static BOOL is_toplevel_instr(char *str,int *cmd_type)
{
	assert(cmd_type);

	if (is_image_instr(str)){
		*cmd_type = IMAGEDEF;
		return TRUE;
	}
	if (is_draw_instr(str)){
		*cmd_type = DRAW;
		return TRUE;
	}
	if (is_print_instr(str)){
		*cmd_type = PRINT;
		return TRUE;
	}
	if (is_translate_instr(str)){
		*cmd_type = TRANSLATE;
		return TRUE;
	}
	if (is_child_instr(str)){
		*cmd_type = CHILD;
		return TRUE;
	}
	if (is_rotate_instr(str)){
		*cmd_type = ROTATE;
		return TRUE;
	}
	return FALSE;
}

static BOOL find_image_name(ImageList * lst,char * name,ImageInfo * ii)
{
	assert(ii);

	if (lst == NULL)return FALSE;
	while (lst){
		if (lst->img && strcmp(lst->img->name,name) == 0){
			*ii = *lst->img;
			return TRUE;
		}
		lst = lst->next;
	}
	return FALSE;
}
static char * lstrip(char * pstr)
{
	if(pstr == NULL)return NULL;
	for (;;){
		if (*pstr == '\0'){
			return pstr;
		}
		if (isspace(*pstr)){
			++pstr;
		}
		else{
			return pstr;
		}
	}
}

static char * copy_str(const char * str)
{
	int len = strlen(str) + 1;
	char * cp = (char*)malloc(len);
	strcpy(cp, str);
	cp[len - 1] = '\0';
	return cp;
}

static char * read_line(char * buf, int max_count, FILE * fp)
{
	char * ret = fgets(buf, max_count, fp);
	++cur_line;
	return ret;
}

static void update_image_info(ImageInfo * ii)
{
	PointList * lst;
	double x, y;

	assert(ii);
	lst = ii->lst;
	if (lst == NULL || lst->next == NULL)return;
	x = lst->pt.x;
	y = lst->pt.y;

	lst = lst->next;
	while (lst){
		lst->pt.x += x;
		lst->pt.y += y;
		x = lst->pt.x;
		y = lst->pt.y;
		lst = lst->next;
	}
}

static int get_image_point_list_length(ImageInfo* ii)
{
	PointList * pl;
	int len = 0;

	assert(ii);
	pl = ii->lst;
	if (pl == NULL)return 0;
	while (pl){
		++len;
		pl = pl->next;
	}
	return len;
}

static void get_image_points(ImageInfo * ii, Point * pts,int len)
{
	int idx = 0;
	PointList * lst;

	assert(ii && pts);
	lst = ii->lst;
	if (len == 0 || lst == NULL)return;
	while (lst){
		pts[idx++] = lst->pt;
		lst = lst->next;
	}
	assert(len == idx);


}
static char * parser_command(char * ptr,char ** out)
{
	int idx = 0;
	char buf[MAXLINE] = { 0 };

	assert(out != NULL && ptr != NULL);

	if (*ptr == '\0'){
		*out = NULL;
		return ptr;
	}
	while (*ptr && isalpha(*ptr)){
		buf[idx++] = *ptr;
		++ptr;
	}
	*out = copy_str(buf);
	return ptr;
}

static char * parser_image_name(char * ptr, char ** out)
{
	int idx = 0;
	char buf[MAXIMAGENAME] = { 0 };

	assert(out != NULL && ptr != NULL);

	if (*ptr == '\0'){
		*out = NULL;
		return ptr;
	}
	while (*ptr && isalnum(*ptr)){
		buf[idx++] = *ptr;
		++ptr;
	}
	*out = copy_str(buf);
	return ptr;
}

static char * pasrer_image_name(char * ptr, char ** out)
{
	int idx = 0;
	char buf[MAXIMAGENAME] = { 0 };

	assert(out != NULL && ptr != NULL);
	if (*ptr == '\0'){
		*out = NULL;
		return ptr;
	}
	while (isalnum(*ptr)){
		buf[idx++] = *ptr;
		++ptr;
	}
	*out = copy_str(buf);
	return ptr;
}

static char * parser_number(char * ptr, double* num)
{
	char * ori = ptr;
	char buf[MAXLINE] = { 0 };
	int idx = 0;
	int has_dot = 0;

	assert(ptr && num);
	if (*ptr == '-'){
		buf[idx++] = '-';
	}
	for (;;){
		if (isspace(*ptr))break;
		if(*ptr == '.'){
			has_dot = 1;
			break;
		}
		if (*ptr && isdigit(*ptr)){
			buf[idx++] = *ptr;
		}
		++ptr;
	}
	if(has_dot){
		for(;;){
			if(isspace(*ptr))break;
			if(*ptr && isdigit(*ptr)){
				buf[idx++] = *ptr;
			}
			else{
				break;
			}
			++ptr;
		}
	}
	if (buf[0] == '\0'){
		exit_and_clean();
	}

	*num = atof(buf);
	return ptr;
}

static void skip_space(const char * ptr)
{
	while (isspace(*ptr)){
		++ptr;
	}
	if (*ptr != 0){
		exit_and_clean();
	}
}

static void add_image(ImageList * il, ImageInfo * ii)
{
	ImageList * tmp;

	if (il->img == NULL && il->next == NULL){
		il->img = ii;
		return;
	}
	else{
		tmp = (ImageList *)malloc(sizeof(ImageList));
		tmp->img = ii;
		tmp->next = il->next;
		il->next = tmp;
	}
}

static void image_add_point(ImageInfo * pii, Point ppt)
{
	PointList * ptr,*tmp;

	assert(pii);

	if (pii->lst == NULL){
		pii->lst = (PointList *)malloc(sizeof(PointList));
		pii->lst->pt = ppt;
		pii->lst->next = NULL;
	}
	else{
		ptr = pii->lst;
		while (ptr){
			if (ptr->next == NULL){
				tmp = (PointList *)malloc(sizeof(PointList));
				tmp->pt = ppt;
				tmp->next = NULL;
				ptr->next = tmp;
				break;
			}
			ptr = ptr->next;
		}
	}

}

static void do_comment(char * str)
{
	fprintf(stdout, "%s", str);
}

// return next valid line
static char * filter_blankline_and_comment(char buf[],int max_count,FILE * fp)
{
	char *ptr = "",*ori;

	for (;;){
		ori = ptr = read_line(buf,max_count, fp);
		ptr = lstrip(ptr);
		if (strlen(ptr) == 0)continue;
		if (*ptr == '#'){
			do_comment(ori);
			//fprintf(stdout, "%s", ori);
			continue;
		}
		return ori;
	}
}

static char* accept(char * ptr, char * target)
{
	char buf[MAXLINE] = { 0 };
	int idx = 0;

	assert(ptr && target);
	while (isalpha(*ptr)){
		buf[idx++] = *ptr++;
	}
	if (strcmp(target, buf) != 0){
		exit_and_clean();
	}

	return ptr;
}

static void print_image(ImageInfo * ii)
{
	PointList  *lst;
	fprintf(stdout, "Print Image %s =\n",ii->name);
	lst = ii->lst;
	while (lst){
		fprintf(stdout, "%2lf %2lf\n", lst->pt.x, lst->pt.y);
		lst = lst->next;
	}
	fprintf(stdout, "End Image %s\n", ii->name);
}

static void draw_image(ImageInfo * ii)
{
	int len,i;
	char buf[MAXLINE] = {0};

	Point * ptrs;
	assert(ii);
	len = get_image_point_list_length(ii);
	if (len == 0 || len == 1)return;
	ptrs = (Point *)malloc(sizeof(Point)* len);
	get_image_points(ii, ptrs, len);

	assert(ske);
	
	for(i = 0; i < len - 1;++i){
		sprintf(buf,"drawSegment %ld %ld %ld %ld\n",(long)ptrs[i].x,(long)ptrs[i].y,
				(long)ptrs[i + 1].x,(long)ptrs[i + 1].y);
		fprintf(ske,"%s",buf);
	}
}

static void translate_image(ImageInfo * ii,double x,double y)
{
	PointList * lst;
	assert(ii);
	lst = ii->lst;
	while(lst){
		lst->pt.x += x;
		lst->pt.y += y;
		lst = lst->next;
	}
}

static void rotate_image(ImageInfo * ii,double radians)
{
	PointList * lst;
	double x,y;
	assert(ii);
	lst = ii->lst;
	while(lst){
		x = lst->pt.x;y = lst->pt.y;
		lst->pt.x = x * cos(radians) - y * sin(radians);
		lst->pt.y = x * sin(radians) + y * cos(radians);
		lst = lst->next;
	}
}

static void do_image_def(char * str)
{
	
	char *image_name,*cmd,*tmp,*ori;
	double num,num1;
	Point * ppt,pt;
	ImageInfo * ii;
	char buf[MAXLINE];

	str = lstrip(str);
	if (strlen(str) == 0){
		exit_and_clean();
	}

	if (!(ii = (ImageInfo *)malloc(sizeof(ImageInfo)))){
		exit_and_clean();
	}
	memset(ii->name, 0, sizeof(ii->name));
	ii->lst = NULL;

	str = lstrip(parser_command(str, &image_name));
	strcpy(ii->name, image_name);

	if (!image_name){
		exit_and_clean();
	}

	str = lstrip(parser_number(str, &num));
	num1 = num;
	str = lstrip(parser_number(str, &num));
	pt.x = num1, pt.y = num;
	image_add_point(ii, pt);

	if (*str++ != '='){
		exit_and_clean();
	}
	skip_space(str);
	
	// parser lineTo instructions
	for (;;){

		// filter blank and comment
		str = filter_blankline_and_comment(buf,sizeof(buf),fp);

		str = lstrip(str);
		str = parser_command(str, &cmd);
		

		if (cmd == NULL){
			exit_and_clean();
		}
		else{
			if (is_lineto_instr(cmd)){
				str = lstrip(parser_number(lstrip(str), &num));
				num1 = num;
				str = lstrip(parser_number(str, &num));
				pt.x = num1, pt.y = num;
				image_add_point(ii, pt);
			}
			else if (is_end_instr(cmd)){
				// End Image image-name
				str = lstrip(str);
				str = lstrip(accept(str, "Image"));
				str = parser_command(str, &tmp);
				if (tmp == NULL){
					exit_and_clean();
				}
				if (strcmp(tmp, image_name) != 0){
					exit_and_clean();
				}

				skip_space(str);
				// end image definitation instruction
				break;
			}
			else{
				exit_and_clean();
			}
		}
	}

	update_image_info(ii);
	add_image(&il, ii);
}

static void do_draw(char * str)
{
	char * image_name;
	ImageInfo ii;

	str = parser_image_name(lstrip(str), &image_name);
	if (image_name == NULL){
		exit_and_clean();
	}
	skip_space(str);
	if (find_image_name(&il,image_name, &ii)){
		draw_image(&ii);
	}
	free(image_name);
}

static void do_print(char * str)
{
	char * image_name;
	ImageInfo ii;

	str = parser_image_name(lstrip(str), &image_name);
	if (image_name == NULL){
		exit_and_clean();
	}

	skip_space(str);
	if (find_image_name(&il,image_name,&ii)){
		print_image(&ii);
	}
	free(image_name);
}

static void do_translate(char * str)
{
	char * image_name;
	ImageList * pil = &il;
	ImageInfo ii;
	double delta_x,delta_y;

	str = parser_image_name(lstrip(str),&image_name);
	if(image_name == NULL){
		exit_and_clean();
	}
	
	str = parser_number(lstrip(str),&delta_x);
	str = parser_number(lstrip(str),&delta_y);

	if(pil->img == NULL){
		exit_and_clean();
	}
	while(pil){
		if(pil->img && strcmp(pil->img->name,image_name) == 0){
			// find image
			translate_image(pil->img,delta_x,delta_y);
			break;
		}
		pil = pil->next;
	}
	// not exist image
	if(pil == NULL){
		free(image_name);
		exit_and_clean();
	}
	free(image_name);
}

static void do_rotate(char * str)
{
	char * image_name;	
	ImageList * pil = &il;
	ImageInfo ii;
	double x,y;
	double degrees,radians;

	str = parser_image_name(lstrip(str),&image_name);
	if(image_name == NULL){
		exit_and_clean();
	}
	
	str = parser_number(lstrip(str),&degrees);
	radians = degrees * M_PI / 180.0;

	if(pil->img == NULL){
		exit_and_clean();
	}
	while(pil){
		if(pil->img && strcmp(pil->img->name,image_name) == 0){
			// find image
			rotate_image(pil->img,radians);
			break;
		}
		pil = pil->next;
	}
	// not exist image
	if(pil == NULL){
		free(image_name);
		exit_and_clean();
	}
	free(image_name);
}

static void do_child(char * str)
{
	str = lstrip(str);
	assert(ske);
	fprintf(ske,"%s",str);
}

Token GetNextToken(void)
{
	Token tok;
	char line[MAXLINE] = { 0 };
	char * orignal_str;
	char * command;
	int cmd_type;

	if (feof(fp)){
		tok.type = END_OF_FILE;
		tok.lexeme = "";
		return tok;
	}
	else{
		cur_strline = read_line(line, MAXLINE, fp);
		if(feof(fp)){
			tok.type = END_OF_FILE;
			return tok;
		}
		orignal_str = cur_strline;
		cur_strline = lstrip(cur_strline);
		if (*cur_strline == '\0'){
			tok.type = BLANK_LINE;
			return tok;
		}
		if (*cur_strline == '#'){
			tok.type = COMMENT;
			do_comment(orignal_str);
			return tok;
		}

		cur_strline = parser_command(cur_strline, &command);
		if (command == NULL){
			error(cur_line);
			exit(EXIT_FAILURE);
		}
		else if (is_toplevel_instr(command,&cmd_type)){
			tok.lexeme = command;
			tok.type = INSTRUCTION;

			switch (cmd_type){
			case IMAGEDEF:
				do_image_def(cur_strline);
				break;
			case DRAW:
				do_draw(cur_strline);
				break;
			case PRINT:
				do_print(cur_strline);
				break;
				
			case TRANSLATE:
				do_translate(cur_strline);
				break;
			case CHILD:
				do_child(cur_strline);
				break;
			case ROTATE:
				do_rotate(cur_strline);
				break;
			default:
				assert(0);
				break;
			}
		}
		else{
			error(cur_line);
			exit(EXIT_FAILURE);
		}

	}
}

BOOL BeginParser(const char * filename)
{
	fp = fopen(filename, "r");
	if (fp == NULL){
		error(0);
		exit(EXIT_FAILURE);
	}
	ske = popen(COMMAND,"w");
	if(ske == NULL){
		error(0);
		exit(EXIT_FAILURE);
	}
}

void EndParser()
{
	if (fp){
		fclose(fp);
		fp = NULL;
	}
	if(ske){
		pclose(ske);
		ske = NULL;
	}
	clean();
}

void parser_file(const char * filename)
{
	BeginParser(filename);
	while(GetNextToken().type != END_OF_FILE)
		;
	EndParser();
}

