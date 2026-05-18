#define string char *
void drawRect(int frame[48][84],int x,int y,int height,int width){
  height--;
  width--;
  for(int j=y;j<height+y+1;j++){
    
  frame[j][x]=1;
  frame[j][x+width]=1;
  if(j==y || j==height+y){
  for(int i=x;i<width+x+1;i++){
      frame[j][i]=1;
       }
    }
  }
}
int staticframebuff[48][84]={0};

struct framebuff{
int x;
int y;
int (*frameset)[84];
struct coords* ptr;
}defaultFrame;

struct coords{
  int x,y;
  struct coords* next;
};


int remaining=84;
int blanknum=0;
string y[]={"01000000n","01000001n","01000010n","01000011n","01000100n","01000101n"};
void sendDC(int ifdata,string bits){
  if(ifdata==1){
  remaining--;}
  string a=bits;
    int i=0;
    gpio_set_level(GPIO_NUM_25,ifdata);
    while(a[i]!='n'){
      if(a[i]=='0'){
      gpio_set_level(GPIO_NUM_14,0);//clockLow
      gpio_set_level(GPIO_NUM_12,0);
      vTaskDelay(1/portTICK_PERIOD_MS);
      gpio_set_level(GPIO_NUM_14,1);
      vTaskDelay(1/portTICK_PERIOD_MS);
      gpio_set_level(GPIO_NUM_14,0);

    }else{
      gpio_set_level(GPIO_NUM_14,0);//clockLow
      gpio_set_level(GPIO_NUM_12,1);
      vTaskDelay(1/portTICK_PERIOD_MS);
      gpio_set_level(GPIO_NUM_14,1);
      vTaskDelay(1/portTICK_PERIOD_MS);
      gpio_set_level(GPIO_NUM_14,0);
    }
    i++;
    }
}
void init(){
    gpio_config_t conf={
    .pin_bit_mask=(1ULL<<GPIO_NUM_4) | (1ULL<<GPIO_NUM_25) | (1ULL<<GPIO_NUM_14) | (1ULL<<GPIO_NUM_15) | (1ULL<<GPIO_NUM_12),
    .mode=GPIO_MODE_OUTPUT,
  };
  gpio_config(&conf);
    //RESET
  gpio_set_level(GPIO_NUM_4,1);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  gpio_set_level(GPIO_NUM_4,0);
  vTaskDelay(80/portTICK_PERIOD_MS);
  gpio_set_level(GPIO_NUM_4,1);
}
void enable(){
  gpio_set_level(GPIO_NUM_15,0);
}
void disable(){
  gpio_set_level(GPIO_NUM_15,1);
}

// Artistic 8x8 font set
// font[LETTER_INDEX][ROW][COL]
//
// Usage:
// font['A'-'A']
// font['B'-'A']
// etc.


void printletter(char letter,bool flag,int startpos){
  
  if(flag && remaining<=0){
    return;
  }
  if(letter==' '){
    sendDC(1,"00000000n");
    sendDC(1,"00000000n");
    sendDC(1,"00000000n");
    sendDC(1,"00000000n");
    return;
  }
  char delim='n';
  if(letter>96){
    letter=letter-32;
  }
  int (*a)[8]=font[letter-65];
  for(int col=startpos;col<8;col++){
    char lettercolset[9]="";
    for(int row=7;row>-1;row--){
      char bit=a[row][col]+'0';
      strncat(lettercolset,&bit,1);
      
    }

    strncat(lettercolset,&delim,1);
    
    sendDC(1,lettercolset);
    if(flag){
      
    if(remaining<=0){
      return;
    }
  }
  }
  sendDC(1,"00000000n");
}

void printLCD(string str){
  int inc=0;
while(str[inc]!='\0'){
  if(remaining<8){
    blanknum++;
    sendDC(0,y[blanknum]);
    sendDC(0,"10000000n");
    remaining=84;
  }
  if(str[inc]==' '){
    sendDC(1,"00000000n");
    sendDC(1,"00000000n");
    sendDC(1,"00000000n");
    sendDC(1,"00000000n");
  }else{
    printletter(str[inc],false,0);
  }
inc++;
}

}
void clearScreen(){
  gpio_set_level(GPIO_NUM_4,0);
  vTaskDelay(80/portTICK_PERIOD_MS);
  gpio_set_level(GPIO_NUM_4,1);
  remaining=84;
  blanknum=0;
}


//--------------------------------------




void marqueetext(string text){
  int letter=0;
  int count=0;
  int firstchar=0;
  while(1){
    
    printletter(text[letter],true,count);
    while(text[letter+1]!='\0'){
    printletter(text[letter+1],true,0);
    letter++;
    }
    vTaskDelay(500/portTICK_PERIOD_MS);
    clearScreen();
    letter=firstchar;
    count+=3;
    if(count>8){
      count=0;
      firstchar++;
      
      if(text[firstchar]=='\0'){
        firstchar=0;
      }
      letter=firstchar;
    }
  }
  
}




//---------------FRAMESET---------------
/*

int staticframebuff[48][84]={0};

struct framebuff{
int x;
int y;
int (*frameset)[84];
struct coords* ptr;
}defaultFrame;

struct coords{
  int x,y;
};

*/
//--------------------------------------
void attach(int (*frame)[84]){
defaultFrame.frameset=frame;
}
void framesetInit(){
  clearScreen();
  attach(staticframebuff);
}
void setCursor(int x,int y){
  defaultFrame.x=x;
  defaultFrame.y=y;

}
void printLetterToFrame(char letter){
  if(letter==' '){
    setCursor(defaultFrame.x+8,defaultFrame.y);
    return;
  }
  if(letter>96){
    letter=letter-32;
  }
  int row=0;
  for (int j=defaultFrame.y;j<8+defaultFrame.y;j++){
  int col=0;  
  for(int i=defaultFrame.x;i<defaultFrame.x+8;i++){
    defaultFrame.frameset[j][i]=font[(int)(letter-'A')][row][col];
    col++;
  }
  row++;
  }
  if(defaultFrame.x+18>83){
  setCursor(0,defaultFrame.y+8);
  }else{
  setCursor(defaultFrame.x+9,defaultFrame.y);
  }
}
void sendframeset(int frame[48][84]);
void display(){
  sendframeset(defaultFrame.frameset);
}

void printToFrame(string text){
  int i=0;
  while(text[i]!='\0'){
    printLetterToFrame(text[i]);
    i++;
  }
}

void drawRectToFrame(int height,int width){
  drawRect(staticframebuff,defaultFrame.x,defaultFrame.y,height,width);
}

void point(int x,int y){
  if(x<0 || y<0 || x>83 || y>47){return;}
  defaultFrame.frameset[y][x]=1;
}
struct coords* head=NULL;
struct coords* tail=NULL;
struct coords* iter=NULL;


void CoordsStore(int x,int y){
if(head==NULL){
  head=(struct coords*)malloc(sizeof(struct coords));
  iter=head;
  head->x=x;
  head->y=y;
  tail=head;
}else{
struct coords* temp=(struct coords*)malloc(sizeof(struct coords));
temp->x=x;
temp->y=y;
tail->next=temp;
tail=temp;
}
tail->next=NULL;
}




struct coords* getCoords(){
  if(iter==NULL){return NULL;}
  struct coords* temp=iter;
  iter=iter->next;
  return temp;
}

void destroyStorage(){
head=NULL;
iter=NULL;
tail=NULL;
}


void plotLineToFrame(int x,int y){
  int ax=x;
  int ay=y;
  int flag=0;
  int x0=defaultFrame.x;
  int y0=defaultFrame.y;
  int newx,newy,dx,dy;
  if(x0>x && y0<y){
    int temp=x0;
    int temp1=y0;
    x0=x;
    y0=y;
    x=temp;
    y=temp1;
  }else if(x0<x && y>y0){
    int temp=x0;
    int temp1=y0;
    x0=x;
    y0=y;
    x=temp;
    y=temp1;
  }
  if(x0>x && y0>y){
flag=1;
x=x0+(x0-x);

}
  dy=abs(y0-y);
  dx=abs(x-x0);
  

  int p;
  if(x>x0 && y<y0){
    if(dy/dx<1){
    CoordsStore(x0,y0);
    newx=x0;
    newy=y0;
    p=2*dy-dx;
    while(newx!=x || newy!=y){
    if(p<0){
    newx=newx+1;
    newy=newy;
    p=p+2*dy;
    }else{
      newx=newx+1;
      newy=newy-1;
      p=p+2*dy-2*dx;
    }
    CoordsStore(newx,newy);
    }
  }else{
    
    CoordsStore(x0,y0);
    newx=x0;
    newy=y0;
    p=2*dx-dy;
    while(newx!=x || newy!=y){
    if(p<0){
    newx=newx;
    newy=newy-1;
    p=p+2*dx;
    }else{
      newx=newx+1;
      newy=newy-1;
      p=p+2*dx-2*dy;
    }
    CoordsStore(newx,newy);
    }


  }
  }

while(1){
if(flag==1){
 struct coords* xp=getCoords();
 if(xp==NULL){break;}
point(x0-(xp->x-x0),xp->y);

}else{
  struct coords* xp=getCoords();
 if(xp==NULL){break;}
point(xp->x,xp->y);
}

}


destroyStorage();
setCursor(ax,ay);
}

void plotTriangleToFrame(int x0,int y0,int x1,int y1){
  int cx=defaultFrame.x;
  int cy=defaultFrame.y;
  plotLineToFrame(x0,y0);
  plotLineToFrame(x1,y1);
  plotLineToFrame(cx,cy);
}


void plotCircleToFrame(int r){
  int x=0;
  int y=r;
  int p=1-r;
    while(x<=y){
      if(p<0){
        x=x+1;
        CoordsStore(x,y);
        p=p+2*x+3;
      }else{
        x=x+1;
        y=y-1;
        CoordsStore(x,y);
        p=p+2*x-2*y+5;
      }
    }
    while(1){
      struct coords* xp=getCoords();
      if(xp==NULL){break;}
      int centx=defaultFrame.x;
      int centy=defaultFrame.y;
      int cx=(xp->x);
      int cy=(xp->y);
      point(cx+centx,cy+centy);
      point(cy+centx,cx+centy);
      point(-cx+centx,cy+centy);
      point(-cx+centx,-cy+centy);
      point(cx+centx,-cy+centy);
      point(-cy+centx,cx+centy);
      point(cy+centx,-cx+centy);
      point(-cy+centx,-cx+centy);
    }
    destroyStorage();

}








//-------------------------------------

void sendframeset(int frame[48][84]){
  //clearScreen();
  int blanksetscan=0;
  
  while(blanksetscan<48){
  for(int col=0;col<84;col++){
    char lettercolset[9]="";
  for(int row=blanksetscan+7;row>blanksetscan-1;row--){
    
      char bita=frame[row][col]+'0';
      strncat(lettercolset,&bita,1);
    
    }
    char delim='n';
  strncat(lettercolset,&delim,1);
  sendDC(1,lettercolset);
    
  }
  blanksetscan+=8;
  }
  
}




