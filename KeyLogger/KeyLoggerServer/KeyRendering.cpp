#include <cstdio>

const char* KeyRender(char temp)
{
    static char buf[20]="";

    switch(temp)
    {
    case '' : case '' : // Ctrl
        sprintf(buf, "[Ctrl]");
        break;

    case '' : // Caps
        sprintf(buf, "[Caps]");
        break;

    case '' : // Shift
        sprintf(buf, "[Shift]");
        break;

    case '' : // Alt
        sprintf(buf, "[Alt]");
        break;

    case 27 : // Esc
        sprintf(buf, "[Esc]");
        break;

    case -64 : // `
        sprintf(buf, "`");
        break;

    case 0xD : // ฟฃลอ
        sprintf(buf, "[Enter]");
        break;

    case -112 :
        sprintf(buf, "[Num]");
        break;

    case -67 :
        sprintf(buf, "-");
        break;

    case -69 :
        sprintf(buf, "=");
        break;

    case -37 :
        sprintf(buf, "[");
        break;

    case -35 :
        sprintf(buf, "]");
        break;

    case -36 :
        sprintf(buf, "\\");
        break;
    case '\b' :
        sprintf(buf, "[Backspace]");
        break;

    case -70 :
        sprintf(buf, ";");
        break;

    case -34 :
        sprintf(buf, "\'");
        break;

    case -68 :
        sprintf(buf, ",");
        break;

    case -66 :
        sprintf(buf, ".");
        break;

    case -65 :
        sprintf(buf, "/");
        break;

    case 'n' : // Del
        sprintf(buf, "[Del]");
        break;

    case '\t' :
        sprintf(buf, "[Tab]");
        break;

    case '`' : // 0
        sprintf(buf, "0");
        break;


    case 'a' : case 'b' : case 'c' : case 'd' :
    case 'e' : case 'f' : case 'g' : case 'h' :
    case 'i' :
        sprintf(buf, "%c", temp - 48);
        break;

    case 'o' :
        sprintf(buf, "/");
        break;

    case 'j' :
        sprintf(buf, "*");
        break;

    case 'm' :
        sprintf(buf, "-");
        break;

    case 'k' :
        sprintf(buf, "+");
        break;

    case '\'' :
        sprintf(buf, "[Right]");
        break;

    case '%' :
        sprintf(buf, "[Left]");
        break;

    case '(' :
        sprintf(buf, "[Bottom]");
        break;

    case '&' :
        sprintf(buf, "[Top]");
        break;

    default:
        sprintf(buf, "%c", temp);

    }

    return buf;
}
