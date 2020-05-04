#include <stdio.h>
#include <string.h>

char gCmd[100] = {0};


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

    return;

}
void getInput()
{
    fgets(gCmd, 2, stdin);

    printf("DEBUG, get cmd: '%s'.\n",gCmd);
    return;
}

void updateLogic()
{
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


    printf("DEBUG, update logic called.\n");
    return;
}

void Display()
{
    int i = 0;
    int j = 0;
    int total = H*V;
    
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

   
    printf("DEBUG, display called.\n");
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

            printf("DEBUG, box[%d](%d,%d), tgt[%d](%d,%d), p(%d,%d), boxOk[%d] = %d, win = %d.\n",\
                        j, gsBoxPos[j].x, gsBoxPos[j].y, j, gsTgtPos[j].x, gsTgtPos[j].y,\
                        gsCurrentPos.x, gsCurrentPos.y, j, boxOk[j], win);
        }

        for(j=0; j<BOX_NUM; j++)
        {
            if(1 != boxOk[j])
            {
                win = 0;
                printf("boxOk[%d] = %d.\n", j, boxOk[j]);
            }
        }

        if(1 == win)
        {
            printf("Congratulations, you get it!\n");
            break;
        }
        else
        {
            printf("still game..\n");
            for(j=0; j<BOX_NUM; j++)
            {
                printf("boxOk[%d] = %d.\n", j, boxOk[j]);
            }
        }

        getInput();

        if(0 == strcmp("q", gCmd))
        {
            printf("cmd equal to 'q'.\n");
            printf("GoodBye~\n");
            break;
        }
        else
        {
            printf("not equal.\n");
        }
        
        updateLogic();
    }

    return 0;
}
