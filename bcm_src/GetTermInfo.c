/***************************************************************************
	                          xmlparse.c
	               version 1.1
	               By RongShou Peng    08.29.2002
	  change by prs     2003.2
	{ ������XmlDelElement������
	  ȥ����XmlGetElement�����н�xml_doc�б�ǩ������ĸ��ת����Сд��ĸ�Ĳ��֡�
	  ������XmlGetElement�����У���ǩֵ������'<'��'>'���ֵ�bug��
	  �޸���xml_find��memicmp�����ĵ��á�
	} 
	change by prs 2004.1
	{
		XmlAddElement�����˵���ʱ�������뱨�ĳ��ȣ��½����Ĳ��ã�
		XmlDelElement�����˵���ʱ�������뱨�ĳ���
		XmlGetElement����һ��������xmldoc_len�Ǳ��ĳ���
		xml_find ����һ������doc_len���������ַ����ĳ���
		
	}
**************************************************************************/

#include <stdlib.h>
#include <string.h>
#include "public.h"
   

#define uchar unsigned char
#define XML_DOC_MAX    8192  /* The max length of the xml_document*/
#define ELE_NAME_MAX   100   /*The max length of every xml_element's  name*/
void xml_strlwr(char *turned);
int  xml_find(uchar *doc, int doc_len, char *find_flag, int find_flag_len, int *find_location);
int  XmlGetElement(uchar *xml_doc, int xmldoc_len, char *ele_name, uchar *ele_value, int va_max_len, int *value_real_len);
int  XmlAddElement(uchar *xml_doc, int xml_doc_max_len, char *ele_name, uchar *ele_value, int value_len, int *xml_real_len);
int  XmlDelElement(uchar *xml_doc, char *ele_name, int *xml_len);




void xml_strlwr(char * turned)
{
	int i;
	i=0;
	while (turned[i]!=0)
	  {
	  	if  ((turned[i]>='A')&&(turned[i]<='Z'))
	  	      turned[i]+=32;
	  	i+=1;
	   }
}
/**************************************************************************
  int xml_find(char * doc,char *find_flag,int find_flag_len,int *find_location)
doc		:	�����ҵ��ַ���
doc_len		:	�����ҵ��ַ����ĳ���
find_flag	:	Ҫ�ҵ��ַ�����find_flag�п������κε��ַ�������'\0'
find_flag_len	:	find_flag�ĳ���
find_location 	:	find_flag�ĵ�һ���ַ� ��doc�е�λ��
**************************************************************************/
int xml_find(uchar *doc, int doc_len, char *find_flag, int find_flag_len, int *find_location)
{
	int flag,i;
	char temp[200];

	flag=0;

	for(i=0;(i<doc_len-find_flag_len+1)&&(flag==0);i++)
	{
		memcpy(temp, doc+i, find_flag_len);
		temp[find_flag_len] = 0;
		xml_strlwr(temp);     
		if (memcmp(temp,find_flag,find_flag_len)==0)
	          	flag=1;
	}
	if (flag)
	{
		*find_location=i-1;
		#ifdef  TESTFIND
			printf("location: ");
			printf("%d\n",*find_location);
	
			printf("find succ");
			printf("%s\n",find_flag);
		#endif
		return(0);
	}
	else
	{
		#ifdef  TESTFIND
			printf("find fail  :"); printf("%s\n",find_flag);
		
		#endif
		return(1);
	}
}

/***************************************************************************
  int XmlGetElement(char * xml_doc,int xmldoc_len,char *ele_name,char * ele_value,int  va_max_len , int *value_real_len)

  ����˵��
 Ruturn  0  XmlGetElement is called successful;
 Return  n  XmlGetElement is called failed ,n is the error code    ;
 xml_doc       : char *      . point to the xml document that will  been parsed;
                               Ҫ������xml document                                
 xmldoc_len    : int         . the xml_document's(xml_doc) length
 			       Ҫ������xml document�ĳ���,�ó����������ڱ��ĵ�ʵ�ʳ���
 ele_name      : char *      . point to the xml element'name that you want to get it's name;
                               Ҫ�����ı�ǩ��,(������'<'��'>')         
 ele_value     : char *      . point to the buffer of you the xml element's value
                               ��Ž��������buffer
 value_real_len: int *       . point to the buffer of the xml element_value's real length;
                               ������buffer��ʵ�ʳ���
 va_max_len    :             . the max element_value's length
                               buffer����󳤶�
*****************************************************************************/

int XmlGetElement(uchar *xml_doc, int xmldoc_len, char *ele_name,uchar *ele_value, int va_max_len, int *value_real_len)
{
	char start[ELE_NAME_MAX+4],end[ELE_NAME_MAX+5];
	int start_len,find_location[2];
	int start_loca,end_loca,xml_doc_len;
	
	xml_doc_len=xmldoc_len;
	if (xml_doc[0]==0) return(201);

	xml_strlwr(ele_name);
	if  (strcmp(ele_name,"root")==0)
	{
		return(202);
	}

	/*�жϱ�ǩ���Ƿ�ΪROOT�����򷵻ش�����2*/
	/*if ele_name equal "ROOT"��return 2*/

	if (strlen(ele_name)>ELE_NAME_MAX)
	{
#ifdef DEBEUGON
		printf("Error!The Element Name is TOO LONG!");
		printf("\n");
#endif
		return(203);
	}

	/*�жϱ�ǩ�������Ƿ��� ���򷵻ش�����3*/
	/*judge the length of element'name is longger then the max or not*/

	if (xml_find(xml_doc,xml_doc_len,"</root>",7,find_location))
		return(204);


	/*��λxml_document�Ľ���λ��*/

	xml_doc_len=find_location[0]+7;
	
	
#ifdef  GETPAUSE
	printf("The xml_doc_len :");

	printf("%d\n",xml_doc_len);
	printf("test1\n");
	getchar(test);
#endif
	if (xml_doc_len>XML_DOC_MAX-1)
	{
#ifdef DEBEUGON
		printf("Error!The Msg is Too LONG!");
		printf("\n");
#endif
		return(205);
	}
	/*�ж�xml document �ĳ����Ƿ��ޣ����򷵻ش����� 5*/
	/****judge the length of xml document is too longger or not */


	strcpy(start,"<");
	strcat(start,ele_name);
	strcat(start,">");
	start_len=strlen(start);

	if (xml_find(xml_doc,xml_doc_len,start,start_len,find_location))
	{
#ifdef DEBUGON
		printf("Error!Not find start ");
		printf("%s",start);
		printf(" in the Msg");
		printf("\n");
#endif
		return(208);
	}

	else if (find_location[0]>xml_doc_len)
	{
#ifdef DEBUGON
		printf("Error!Not find start ");
		printf("%s",start);
		printf(" in the Msg");
		printf ("\n");
#endif
		return(209);
	}

	start_loca=find_location[0];
#ifdef GETPAUSE
printf("%d\n",start_loca);
#endif
	/*��λ<��ǩ��> ���û���ҵ�,���ش�����9*/
	/*find the "<"+*ele_name+">"  . if not find return 1*/

	strcpy(end,"</");
	strcat(end,ele_name);
	strcat(end,">");
	start_len=strlen(start);


	if (xml_find(xml_doc,xml_doc_len,end,strlen(end),find_location))
	{
#ifdef DEBUGON
		printf("Error!Not find end");
		printf("%s",end);
		printf(" in the Msg");
		printf("\n");
#endif
		return(210);
	}

	else if (find_location[0]>xml_doc_len)
	{
#ifdef DEBUGON
		printf("Error!Not find end");
		printf("%s",end);
		printf(" in the Msg");
		printf("\n");
#endif
		return(211);
	}

	end_loca=find_location[0];
	/* ��λ</��ǩ��>,���û���ҵ�,���ش�����11*/
	/*find the "</"+*ele_name+">"  . if not find return error code 11*/

	*value_real_len=end_loca-start_loca-strlen(start);
	if (*value_real_len<0 )
	{
#ifdef DEBUGON
		printf("Error!End is before Start\n");
#endif
		return(212);
	}

	if (*value_real_len>va_max_len)
	{
#ifdef DEBUGON
		printf("Error!The Element Value's Length >");
		printf("%s\n",va_max_len);
#endif
		return(213);
	}

	memcpy(ele_value,xml_doc+start_loca+start_len,*value_real_len);
	ele_value[*value_real_len] = 0;
	
#ifdef DEBUGON
	printf("The Element length is : ");
	printf("%d\n",*value_real_len);
	printf("The Element Value is : ");

	for(i=0;i<*value_real_len;i++)
	printf("%c",ele_value[i]);
	printf("\n");
	printf("\n");
#endif

	return(0);
	/*�������������0*/
	/* copy the ELEMENT  VALUE to ele_value*/
}

/*******************************************************************************************
  int XmlAddElement(char * xml_doc,char *ele_name,char *ele_value,int value_len,int *xml_real_len)

  ����˵��
  xml_doc         : xml document ��
  xml_doc_max_len : xml_doc buffer��󳤶�
  ele_name        : �����root���½�һ��xml document��xml_doc ָ���xml document��value_len,ele_value���⣬
                    ��xml_real_len ��Ϊ�½���xml document ��ʵ�ʳ��ȡ�
                    �������root����xml_doc���</root>ǰ)����һ����ǩ�ͱ�ǩֵΪele_value ��ǰvalue_len���ַ���
  ele_value       : Ҫ�½��ı�ǩֵ��ָ�롣
  value_len       : �½���ǩֵ���ܳ��ȣ����򿽱�ele_value�е�value_len��xml_doc��
  xml_real_len    : xml_doc ���ܳ��� ����ʱʹ����xml_doc�ĳ��ȣ����óɹ��󷵻ص���xml_doc���³��ȡ�

*******************************************************************************************/
int XmlAddElement(uchar *xml_doc, int xml_doc_max_len, char *ele_name, uchar *ele_value, int value_len, int *xml_real_len)
{
	char *insert_location;
	int name_len,xml_doc_len,find_location[1];
	char tmp[ELE_NAME_MAX+1];

	
	if (ele_name[0]==0) return (101);

	name_len=strlen(ele_name);
	xml_strlwr(ele_name);
	if(name_len>ELE_NAME_MAX)
	{
		#ifdef   DEBUGON
		printf("ERROR!The Element's Name is TOO LONG or It is not end by normal!\n");
		#endif
		return(102);
	}
	if  (value_len>XML_DOC_MAX)
	{
		#ifdef   DEBUGON
		printf("The length of element value is TOO LONG\n");
		#endif
		return(103);
	}

	if ((strcmp(ele_name,"root")==0)||(strcmp(ele_name,"ROOT")==0))
	{
		strcpy(xml_doc,"<?xml version=\"1.0\"?><root></root>");
		*xml_real_len=strlen("<?xml version=\"1.0\"?><root></root>");
		return(0);
	}
	else
	{
		xml_doc_len=*xml_real_len;/*add by prs 20040116 */
		if ((xml_doc_len>XML_DOC_MAX)||(xml_doc_len<0))
		{	
			#ifdef   DEBUGON
			printf("ERROR!The xml_document length=[%d] is 0 too long !\n",xml_doc_len);
			#endif
			return(100);
		}	
		if (xml_doc[0]==0)
		{
			#ifdef   DEBUGON
			printf("ERROR!The xml_document is NULL!\n");
			#endif
			return(104);
		}


		if (xml_find(xml_doc,xml_doc_len,"</root>",7,find_location))
			return(105);

		xml_doc_len=find_location[0]+7;

		if (name_len+name_len+5+value_len+xml_doc_len>XML_DOC_MAX)
		{
			#ifdef   DEBUGON
			printf("ERROR!The Length of xml_document will TOO LONG!\n");
			#endif
			return(106);
		}
		memcpy(tmp,"<",1);
		memcpy(tmp+1,ele_name,name_len);
		memcpy(tmp+1+name_len,">",1);
		tmp[name_len+2]=0;
		if (!xml_find(xml_doc,xml_doc_len,tmp,2+name_len,find_location))
			{
				#ifdef   DEBUGON
				printf("ERROR!The Length of xml_element_name repeat !\n");
				#endif
				return(107);
			}
		/* �ж��Ƿ��ǩ���Ƿ��Ѿ����� */
	
			if (name_len+name_len+5+value_len+xml_doc_len>xml_doc_max_len)
			{
				#ifdef   DEBUGON
				printf("ERROR!xml_doc over flow!\n");
				#endif
				return(108);
			}	

		*xml_real_len=name_len+name_len+5+value_len+xml_doc_len;
		
		insert_location=xml_doc+find_location[0];

		/*��λָ�뵽xml�ĵ������*/

		memcpy(insert_location,"<",1);
		insert_location+=1;
		memcpy(insert_location,ele_name,name_len);
		insert_location+=name_len;
		memcpy(insert_location,">",1);
		insert_location+=1;
		/*����µı�ǩ�� add new element name start*/

		memcpy(insert_location,ele_value,value_len);
		insert_location+=value_len;
		/*����µ�ֵ,add new element value*/
		memcpy(insert_location,"</",2);
		insert_location+=2;
		memcpy(insert_location,ele_name,name_len);
		insert_location+=name_len;
		memcpy(insert_location,">",1);
		insert_location+=1;
		/*����±�ǩ���Ľ����� add new element name end*/
		memcpy(insert_location,"</root>",7);
		insert_location+=7;
		insert_location[0]=0;

		return(0);
	}
}


/**************************************************************************
  int XmlDelElement(char * xml_doc,char * ele_name, int * xml_len)
      input:    xml_doc        :    �����xml����;
                ele_name       :    Ҫɾ���ı�ǩ������������<>);
                xml_len        :    ����ǰxml_doc���ȣ����غ�ɾ����ǩele_name��xml_doc �ĳ���
      out  :    ɾ���ɹ�����0�����򷵻ض�Ӧ�Ĵ����룬�ɼ���DEBUG_DEL �鿴������Ϣ
           
**************************************************************************/
int XmlDelElement(uchar *xml_doc, char *ele_name, int *xml_len)
{
	char ele_start[ELE_NAME_MAX+5],ele_end[ELE_NAME_MAX+5];
	int  ele_name_len,name_start,name_end,root_end,xml_doc_len;
	xml_doc_len=*xml_len;
	if ((xml_doc_len>XML_DOC_MAX)||(xml_doc_len<0))
	{
		#ifdef   DEBUGON
		printf("ERROR!The xml_document length=[%d] is 0 too long !\n",xml_doc_len);
		#endif
		return(300);
	}	

	if (xml_find(xml_doc,xml_doc_len,"</root>",7,&root_end))
	{ 
		#ifdef DEBUG_DEL
		        printf("ERROR ON 21 in xml_del_name! \nNot find</ROOT>\n");
		#endif 	   
		return(304); 

        }
	xml_doc_len=root_end+7;

	#ifdef DEBUG_DEL
	        printf("%s\n",ele_name);
	#endif 
        
	ele_name_len=strlen(ele_name);
	if  (ele_name_len>ELE_NAME_MAX)
	{

		#ifdef DEBUG_DEL
		        printf("ERROR ON 22 in xml_del_name! \nThe ele_name is too long!\n");
		#endif  
		return (301);
	}
	strcpy(ele_start,"<");
	strcat(ele_start,ele_name);
	strcat(ele_start,">");
	ele_start[2+ele_name_len]=0;
	
	memcpy(ele_end,"</",2);
	memcpy(ele_end+2,ele_name,ele_name_len);
	memcpy(ele_end+2+ele_name_len,">",1);
	ele_end[3+ele_name_len]=0;

	if (xml_find(xml_doc,xml_doc_len,ele_start,2+ele_name_len,&name_start))
	{
		#ifdef DEBUG_DEL
		        printf("ERROR ON 19 in xml_del_name! \nNot find %s\n",ele_start);
		#endif 
	        return(302);
	}
	if (xml_find(xml_doc,xml_doc_len,ele_end,3+ele_name_len,&name_end))
	{ 

		#ifdef DEBUG_DEL
		        printf("ERROR ON 20 in xml_del_name! \nNot find [%s]\n",ele_end);
		#endif 	   
	   	return(303);
	}

	memcpy(xml_doc+name_start,xml_doc+name_end+3+ele_name_len,root_end+7-(name_start+3+ele_name_len));
	*xml_len=root_end+7-(name_end+3+ele_name_len-name_start);
	xml_doc[*xml_len]=0;
	return(0);
}


