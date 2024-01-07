String split[50];
int split_size=0;
int value=255;
//get the x,y,e,f value from the command
double x_val;
double y_val;
double z_val;
double e_val;
double f_val;

//initialize the required variables:
//HOME COMMAND
bool G28=false;

//MACHINE MODE ABSOLUTE OR RELATIVE COORDINATES:

//ABSOLUTE MODE FOR XYZ
bool G90=true;

//RELATIVE MODE FOR XYZ:
bool G91=false;

//LINEAR MOVEMENT
bool G1=false;

//
bool G0=true;

//SET CURRENT POSITION:
bool G92=false;

//SET THE UNITS 
//MM
bool G20=true;
//inch
bool G21=false;

//EXTRUDER HEATING COMMANDS:(M104)(M109)
//waiting to reach the temperature:
bool M104=false;
//doesnot wait to reach the temperature:
bool M109=true;

//BED HEATING COMMANDS:(M140)(M190)
//waiting to reach the temperature:
bool M140=false;
//doesnot wait to reach the temperature:
bool M190=true;

//EXTRUDER POSITION MODE:
bool M82=false;
bool M83=true;

//MOTOR STATE:
 bool M84=true;


bool isalpha(String a){
  for(int i=0;i<a.length();i++){
    if(!isAlpha(a[i])){
      return false;
    }
  }
  return true;
}


/*SPEED FINDER FOR EACH MOTOR:



*/
float speed[3];
void scale(int x,int y,int z){
  float maximum=max(max(x,y),z);
  speed[0]=(x/maximum)*255;
  speed[1]=(y/maximum)*255;
  speed[2]=(z/maximum)*255;
}
//REMOVE THE COMMENTS IN THE GCODE:
String remove_comment(String line){
  String gcoder="";
 int line_size=line.length();
  bool colon=false;
  bool paren=false;
  for(int i=0;i<line_size;i++)
  {
    if(line[i]==';')
      colon=true;
    else if(line[i]=='(')
      paren=true;
  }
  if(colon){
    for(int i=0;i<line_size;i++)
    { if(line[i]==';')
      return gcoder;
     else
      gcoder+=line[i];
    }
  }
  else if(paren){
    for(int i=0;i<line_size;i++){
      bool n=false;
      if(line[i]=='(')
        n=true;
      else if(line[i]==')')
        n=false;
      if(n){
        gcoder+=line[i];}
    
    
  }
   return gcoder;
}
  else if((colon==0)&&(paren==0))
    return line;
}
void token(String gcode)
{ 
  int gcode_size=gcode.length();
  if(isAlpha(gcode[gcode_size])==false)
     {
       gcode+="A";
    gcode_size=gcode.length();
     }
  if(isAlpha(gcode[gcode_size])==true)
     {
       gcode+="1";
    gcode_size=gcode.length();
     }
  
  split_size=0;
  String word="";
  String num="";
  char prev='\0';
  for (int i=0;i<gcode_size;i++)
  {
    if(isAlpha(gcode[i]))
    {
      word+=gcode[i];
    }
    else
    {
      num+=gcode[i];
    }
    if((isAlpha(gcode[i])==true)&&(isAlpha(prev)==false))
    {

      split[split_size]=num;
      num="";
      split_size++;
    }
    else if((isAlpha(gcode[i])==false)&&(isAlpha(prev)==true))
    {
      split[split_size]=word;
      word="";
      split_size++;
    }
    prev=gcode[i];
  }



}
void gcode_compiler(String line){
line=remove_comment(line);
  token(line);
String prev="";
 
for(int i=0;i<split_size;i++){
//GET THE  X VALUE FROM THE COMMAND
if(split[i]=="X"){
//get the X value
if(!isalpha(split[i+1])){
x_val=split[i+1].toDouble();
  Serial.println("x value is:");
  Serial.println(x_val);
}  
}
//GET THE Y VALUE FROM THE COMMAND
else if(split[i]=="Y"){
//get the Y value
if(!isalpha(split[i+1])){
y_val=split[i+1].toDouble();
  
  Serial.println("y value is:");
  Serial.println(y_val);
}  
}
//GET THE   Z VALUE FROM THE COMMAND
else if(split[i]=="Z"){
//get the Z value
if(!isalpha(split[i+1])){
z_val=split[i+1].toDouble();
  Serial.println("z value is:");
  Serial.println(z_val);
}  
}


//GET THE EXTRUDER VALUE FROM THE COMMAND
else if(split[i]=="E"){
//get the E value
if(!isalpha(split[i+1])){
e_val=split[i+1].toDouble();
  Serial.println("e value is:");
  Serial.println(e_val);
}  
}


//GET THE F VALUE FROM THE COMMAND:
else if(split[i]=="F"){
//get the F value
if(!isalpha(split[i+1])){
f_val=split[i+1].toDouble();
  
  Serial.println("f value is:");
  Serial.println(f_val);
}  
}

//establishment of M followed commands
else if((split[i]=="M")&&(isalpha(split[i+1])==false)){
  //custom speed set
  Serial.println("M COMMAND FOUND");
  
if(split[i+2]=="S"){
  if(isalpha(split[i+3])==false){
value=split[i+3].toInt();
 Serial.println("Set Found");
  }  
 
}
if((i+2)>=split_size){
  Serial.println("Setting the maximum value default:");
    value=255;}
//extruder position mode
  
//ABSOLUTE EXTRUDE MODE
 if(split[i+1]=="82"){
    M82=true;
    M83=false;
   Serial.println("Extruder:Absolute Mode");
  }
  
//RELATIVE EXTRUDE MODE:
if(split[i+1]=="83"){
    M82=false;
    M83=true;
  Serial.println("Extruder:Relative Mode");
  }
//disable the motor:
if(split[i+1]=="84"){
    M84=true;
  Serial.println("DISABLE:Motor");
  }
//bed
if(split[i+1]=="140")
{
  M140=true;
  M190=false;
  Serial.println("BED IS HEATING NOW");
  Serial.println(value);
  Serial.println("WAITING");
}
if(split[i+1]=="190")
{
  M140=false;
  M190=true;
  Serial.println("BED IS HEATING NOW");
  Serial.println(value);
  Serial.println("CONTINUE");
}
//extruder
if(split[i+1]=="104")
{
  M104=true;
  M109=false;
  Serial.println("EXTRUDER IS HEATING NOW");
  Serial.println(value);
  Serial.println("WAITING");
}
if(split[i+1]=="109")
{
  M104=false;
  M109=true;
  Serial.println("EXTRUDER IS HEATING NOW");
  Serial.println(value);
  Serial.println("CONTINUE");
}
if(split[i+1]=="106")
{
///set the fan speed
  Serial.println("FAN SPEED IS  NOW");
  Serial.println(value);
 
}
if(split[i+1]=="107")
{
///set the fan speed to 0:
  Serial.println("FAN SPEED SET TO 0");
  
 
}


}
//establishment of g commands
else if((split[i]=="G")&&(isalpha(split[i+1])==false)){
if (split[i+1]=="1")
{
  G1=true;
  G0=false;
  Serial.println("Print Move");
}
else if(split[i+1]=="0")
{
  G1=false;
  G0=true;
  Serial.println("Travel Move");
}
else if(split[i+1]=="20"){
  G20=true;
  G21=false;
  Serial.println("Set units to MM");
}
else if(split[i+1]=="21"){
  G21=false;
  G20=true;
  Serial.println("Set Units to Inch");
}
else if(split[i+1]=="92"){
  G92=true;
  Serial.println("Set to New Position");

}
else if(split[i+1]=="90"){
  G90=true;
  Serial.println("Absolute Mode to XYZ COORDINATES");

}
else if(split[i+1]=="91"){
  G91=true;
  Serial.println("RELATIVE Mode to XYZ COORDINATES");

}
else if(split[i+1]=="28"){
  G28=true;
  Serial.println("HOME TO (0 , 0)");

}

}



//LOOP END
prev=split[i];
}
  return;

}
void setup(){
  Serial.begin(9600);
  Serial.println("Starting the program:");
//gcode_compiler("X10Y20Z30;HI");
 

  
}
void loop(){
  if(Serial.available()){
    String data=Serial.readStringUntil('\n');
    data.replace(" ","");
    Serial.print("splited lines:");
    token(data);
    for(int i=0;i<split_size;i++){
      split[i].replace(" ","");
      Serial.print(split[i]);
      Serial.print(",");
    }
    Serial.println();
gcode_compiler(data);
  }
}

/*TESTING THIS PROGRAM IN TKINTER PROGRAM SOFTWare 10/11/2023
1:40

*/