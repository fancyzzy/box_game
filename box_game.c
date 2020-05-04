#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <sys/select.h>
static void sleep_ms(unsigned int secs)
{
    struct timeval tval;
    tval.tv_sec = secs/1000;
    tval.tv_usec = (secs*1000)%1000000;
    select(0, NULL, NULL, NULL, &tval);
}

// fgets will get the '\n' as the second last character
// so 'DEBUG OFF' count 11 bytes
char gCmd[11] = {0};

int Debug = 0;

#define PRINT(fmt...)\
    do\
    {\
        if(1 == Debug)\
        {\
            printf("[DEBUG] " fmt);\
        }\
    }while(0)

const int H = 6;
const int V = 6;

char gMap[H*V] = {'#'};

typedef struct
{
    int x;
    int y;
}POS_S;

POS_S gsCurrentPos;

const int BOX_NUM = 2;
POS_S gsBoxPos[BOX_NUM];
POS_S gsTgtPos[BOX_NUM];

void generatePos()
{
    // player
    // x >= 1, x < H
    // y >= 1, y < V
    srand(time(NULL));
    gsCurrentPos.x = 1 + rand()%(H-2);
    gsCurrentPos.y = 1 + rand()%(V-2);

    // target
    // same as player, but each should be different
    int usedX[BOX_NUM] = {0};
    int usedY[BOX_NUM] = {0};
    memset(usedX, 0, sizeof(usedX));
    memset(usedY, 0, sizeof(usedY));
    int j = 0;
    int k = 0;
    int randomX = 0;
    int randomY = 0;
    int duplicate = 0;
    for(j=0; j<BOX_NUM; j++)
    {
        do
        {
            //sleep(1);
            sleep_ms(500);
            duplicate = 0; 
            srand(time(NULL));
            randomX = 1 + rand()%(H-2);
            randomY = 1 + rand()%(V-2);

            PRINT("for gen tgt, randomX: %d, randomY: %d.\n",\
            randomX, randomY);
            for(k=0; k<BOX_NUM; k++)
            {
                if(usedX[k] == randomX && usedY[k] == randomY)
                {
                    PRINT("found duplicate, usedX[%d]: %d, usedY[%d]: %d.\n", \
                    k, usedX[k], k, usedY[k]);
                    duplicate = 1;
                }
            }
            if(1 != duplicate)
            {
                gsTgtPos[j].x = randomX;
                gsTgtPos[j].y = randomY;
                usedX[j] = randomX;
                usedY[j] = randomY;
            }
        }while( 1== duplicate);
    }

    // box
    int usedBoxX[BOX_NUM] = {0};
    int usedBoxY[BOX_NUM] = {0};
    memset(usedBoxX, 0, sizeof(usedBoxX));
    memset(usedBoxY, 0, sizeof(usedBoxY));
    for(j=0; j<BOX_NUM; j++)
    {
        do
        {
            duplicate = 0;
            srand(time(NULL));
            randomX = 2 + rand()%(H-4);
            randomY = 2 + rand()%(V-4);
            PRINT("for gen box, randomX: %d, randomY: %d.\n",\
            randomX, randomY);

            for(k=0; k<BOX_NUM; k++)
            {
                if(usedBoxX[k] == randomX && usedBoxY[k] == randomY)
                {
                    PRINT("found duplicate, usedBoxX[%d]: %d, usedBoxY[%d]: %d.\n", \
                    k, usedBoxX[k], k, usedBoxY[k]);
                    duplicate = 1;
                }
            }

            if(gsCurrentPos.x == randomX && gsCurrentPos.y == randomY)
            {
                PRINT("found duplicate, gsCurrentPos.x: %d, gsCurrentPos.y: %d.\n", \
                gsCurrentPos.x, gsCurrentPos.y);
                duplicate = 1;
            }

            if(1 != duplicate)
            {
                gsBoxPos[j].x = randomX;
                gsBoxPos[j].y = randomY;
                usedBoxX[j] = randomX;
                usedBoxY[j] = randomY;
            }

        }while(1 == duplicate);
    }

    return;
}

void gameInit()
{
    gsCurrentPos.x = 2;
    gsCurrentPos.y = 3;
    
    gsBoxPos[0].x = 2;
    gsBoxPos[0].y = 2;
    gsBoxPos[1].x = 3;
    gsBoxPos[1].y = 3;

    gsTgtPos[0].x = 1;
    gsTgtPos[0].y = 1;
    gsTgtPos[1].x = 3;
    gsTgtPos[1].y = 4;

    // generate auto map
    generatePos();
    return;

}
void getInput()
{
    PRINT("getInput called.\n");
    printf("Your input-> ");
    fgets(gCmd, 11, stdin);

    int k = 0;
    for(k=0; k<11; k++)
    {
        PRINT("original gCmd[%d] = '%c' ", k, gCmd[k]);
    }
    // make the last '\n' to '\0'
    gCmd[strlen(gCmd)-1] = '\0';
    PRINT("get cmd: '%s', len: %lu.\n",gCmd, strlen(gCmd));
    printf("\n");
    if(0 == strcmp("DEBUG ON", gCmd))
    {
        Debug = 1;
    }
    else if(0 == strcmp("DEBUG OFF", gCmd))
    {
        Debug = 0;
    }
    else
    {
        //do nothing
    }
    return;
}

void updateLogic()
{
    PRINT("updateLogci called.\n");
    PRINT("gCmd: '%s'.\n", gCmd);
    if('\n' == gCmd[0])
    {
        return;
    }
    int pTop = (gsCurrentPos.x-1)*V + gsCurrentPos.y;
    int pBot = (gsCurrentPos.x+1)*V + gsCurrentPos.y;
    int pLeft = gsCurrentPos.x*V + gsCurrentPos.y - 1;
    int pRight = gsCurrentPos.x*V + gsCurrentPos.y + 1;

    int pBoxTop = (gsCurrentPos.x-2)*V + gsCurrentPos.y;
    int pBoxBot = (gsCurrentPos.x+2)*V + gsCurrentPos.y;
    int pBoxLeft = gsCurrentPos.x*V + gsCurrentPos.y - 2;
    int pBoxRight = gsCurrentPos.x*V + gsCurrentPos.y + 2;

    int j = 0;
    if(0 == strcmp("w", gCmd))
    {
        if(' ' == gMap[pTop] || '.' == gMap[pTop])
        {
            gsCurrentPos.x -= 1;
        }
        else if('@' == gMap[pTop])
        {
            if(' ' == gMap[pBoxTop] || '.' == gMap[pBoxTop])
            {
                for(j=0; j<BOX_NUM; j++)
                {
                    if(gsBoxPos[j].x == gsCurrentPos.x - 1 && gsBoxPos[j].y == gsCurrentPos.y)
                    {
                        gsBoxPos[j].x -=1;
                    }
                }
                gsCurrentPos.x -= 1;
            }
        }
        else // can't move
        {
            // do nonthing
        }
        
    }
    else if(0 == strcmp("s", gCmd))
    {
        if(' ' == gMap[pBot] || '.' == gMap[pBot])
        {
            gsCurrentPos.x += 1;
        }
        else if('@' == gMap[pBot])
        {
            if(' ' == gMap[pBoxBot] || '.' == gMap[pBoxBot])
            {
                for(j=0; j<BOX_NUM; j++)
                {
                    if(gsBoxPos[j].x == gsCurrentPos.x + 1 && gsBoxPos[j].y == gsCurrentPos.y)
                    {
                        gsBoxPos[j].x +=1;
                    }
                }
                gsCurrentPos.x += 1;
            }
        }
        else // can't move
        {
            // do nonthing
        }
    }
    else if(0 == strcmp("a", gCmd))
    {
        if(' ' == gMap[pLeft] || '.' == gMap[pLeft])
        {
            gsCurrentPos.y -= 1;
        }
        else if('@' == gMap[pLeft])
        {
            if(' ' == gMap[pBoxLeft] || '.' == gMap[pBoxLeft])
            {
                for(j=0; j<BOX_NUM; j++)
                {
                    if(gsBoxPos[j].y == gsCurrentPos.y - 1 && gsBoxPos[j].x == gsCurrentPos.x)
                    {
                        gsBoxPos[j].y -=1;
                    }
                }
                gsCurrentPos.y -= 1;
            }
        }
        else // can't move
        {
            // do nonthing
        }
    }
    else if(0 == strcmp("d", gCmd))
    {
        if(' ' == gMap[pRight] || '.' == gMap[pRight])
        {
            gsCurrentPos.y += 1;
        }
        else if('@' == gMap[pRight])
        {
            if(' ' == gMap[pBoxRight] || '.' == gMap[pBoxRight])
            {
                for(j=0; j<BOX_NUM; j++)
                {
                    if(gsBoxPos[j].y == gsCurrentPos.y + 1 && gsBoxPos[j].x == gsCurrentPos.x)
                    {
                        gsBoxPos[j].y +=1;
                    }
                }
                gsCurrentPos.y += 1;
            }
        }
        else // can't move
        {
            // do nonthing
        }
    }
    else
    {
        printf("unsupported move: '%s'.\n", gCmd);
    }


    PRINT("update logic called.\n");
    return;
}

void Display()
{
    int i = 0;
    int j = 0;
    int total = H*V;
    
    PRINT("Display called.\n");
    //  calculate  position
    for(i=0; i<total; i++)
    {
        if( i < V || i > (H-1)*V || 0 == i%H || 0 == (i+1)%H )
        {
            gMap[i] = '#';
        }
        else
        {
            gMap[i] = ' ';
        }

        // target
        for(j=0; j<BOX_NUM; j++)
        {
            if(i == gsTgtPos[j].x*V + gsTgtPos[j].y)
            {
                gMap[i] = '.';
            }
            else
            {
                // do nothing
            }
        }

        // box
        for(j=0; j<BOX_NUM; j++)
        {
            if(i == gsBoxPos[j].x*V + gsBoxPos[j].y)
            {
                gMap[i] = '@';
            }
        }

        // player
        if(i == gsCurrentPos.x*V + gsCurrentPos.y)
        {
            gMap[i] = 'P';
        }
    }

    // show
    for(i = 0; i < total; i++)
    {
        printf("%c",gMap[i]);
        if( 0 == (i+1)%V )
        {
            printf("\n");
        }
    }

    return;
}

int main(void)
{
    int j = 0;
    int k = 0;
    int boxOk[BOX_NUM];
    for(j=0; j<BOX_NUM; j++)
    {
        boxOk[j] = 0;
    }
    int win = 1;

    gameInit();

    while(1)
    {
        win = 1;
        Display();
        // check win
        for(j=0; j<BOX_NUM; j++)
        {
            for(k=0; k<BOX_NUM; k++)
            {
                if(gsBoxPos[j].x == gsTgtPos[k].x && gsBoxPos[j].y == gsTgtPos[k].y)
                {
                     boxOk[j] = 1;
                     break;
                }
            }

            PRINT("DEBUG, box[%d](%d,%d), tgt[%d](%d,%d), p(%d,%d), boxOk[%d] = %d, win = %d.\n",\
                        j, gsBoxPos[j].x, gsBoxPos[j].y, j, gsTgtPos[j].x, gsTgtPos[j].y,\
                        gsCurrentPos.x, gsCurrentPos.y, j, boxOk[j], win);
        }

        for(j=0; j<BOX_NUM; j++)
        {
            if(1 != boxOk[j])
            {
                win = 0;
                PRINT("boxOk[%d] = %d.\n", j, boxOk[j]);
            }
        }

        if(1 == win)
        {
            printf("Congratulations, you get it!\n");
            break;
        }
        else
        {
            for(j=0; j<BOX_NUM; j++)
            {
                PRINT("game going on..., boxOk[%d] = %d.\n", j, boxOk[j]);
            }
        }

        getInput();

        if(0 == strcmp("q", gCmd))
        {
            PRINT("cmd equal to 'q'.\n");
            printf("GoodBye~\n");
            break;
        }
        else
        {
            //do nonthing
        }
        
        updateLogic();
    }

    return 0;
}
