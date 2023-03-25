#include "screen.h"

void color(int i, int highlight, ville *villes, WINDOW *win)
{
    start_color();

    init_pair(1, COLOR_BLACK, COLOR_GREEN);
    init_pair(2, COLOR_BLACK, COLOR_RED);
    init_pair(3, COLOR_GREEN, COLOR_YELLOW);
    init_pair(4, COLOR_RED, COLOR_YELLOW);

    if (villes[i].cible)
    {
        if (i == highlight)
            wattron(win, COLOR_PAIR(4));
        else
            wattron(win, COLOR_PAIR(2));
    }
    else
    {
        if (i == highlight)
            wattron(win, COLOR_PAIR(3));
        else
            wattron(win, COLOR_PAIR(1));
    }
}

void afficherMap();

//------------------main------------------//
int main()
{   
    //------------------init------------------// 
    if (!bcm2835_init())
        return 1;

    initscr(); /* Start curses mode 		  */
    noecho();
    cbreak();

    initMatrice();

    //------------------déclaration ------------------// 
    ville choices[10] = {
    {" Los Angeles ", 12, 13, 0},
    {" New York ", 10, 53, 0},
    {" Washington ", 11, 48, 0},
    {" Londres ", 8, 69, 0},
    {" Paris ", 9, 82, 0},
    {" Berlin ", 8, 87, 0},
    {" Madrid ", 11, 72, 0},
    {" Moscou ", 7, 95, 0},
    {" Sydney ", 29, 149, 0},
    {" Tokyo ", 11, 144, 0}};

    WINDOW *win;
    int choice;
    int highlight = 0;
    int i;
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);


    win = newwin(yMax, xMax, 0, 0);
    box(win, 0, 0);
    refresh();
    wrefresh(win);

    afficherMap(win);

    while (1)
    {   
        wrefresh(win);
        // Affichage des choix
        for (i = 0; i < 10; i++)
        {
            color(i, highlight, choices, win);
            mvwprintw(win, choices[i].coorX, choices[i].coorY, "%s", choices[i].nom);
        }
        wattroff(win, COLOR_PAIR(1));

        choice= scanMatrix();
        
        switch (choice)
        {
        case 0:
            highlight--;
            if (highlight == -1)
                highlight = 9;
            break;
        case 3:
            highlight++;
            if (highlight == 10)
                highlight = 0;
            break;
        case 2:
            choices[highlight].cible = !choices[highlight].cible;
            break;
        default:
            break;
        }
        if (choice == 1)
            break;
    }
    
    endwin(); /* End curses mode		  */
    
    bcm2835_close();
    return 0;
}

void afficherMap(WINDOW *win)
{
    wprintw(win, "                                       .____________,___ .__________ .                                                                                       \n");
    wprintw(win, "                             ._,  .__oooooooooroooooooooooooooooooooooor`            |Jooooo`      ````                  ooo___                              \n");
    wprintw(win, "                        _oo_ooo7L oooooo, ````      ``7ooooooooooooooor`              `````             ._ ````       ___ooJooooor`          Jo____          \n");
    wprintw(win, "_,      _______,,     _,oor`ooooooooooor`oooooL___     |oooooooooooooo,                  .___,        |oo    .Jo|ooooooooooooooooooooooooL___JooooL____,   . \n");
    wprintw(win, "oL__ .oooooooooooooooooooooooooooooLoooooJ`` |ooooL_,   ooooooooo`   _,.._            .oooooooooL_.oo__Jooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
    wprintw(win, "```7L,__ooooooooooooooooooooooooooooooo``JooL__o`oo     `7ooo`       .oor`        ._Jooo`LJoooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
    wprintw(win, "     |r``7or````````oooooooooooooooooo_     `7oooLJL,     ````               ._   ``7oor``7_Joooooooooooooooooooooooooooooooooooooooooooooooooor``````_Jr``` \n");
    wprintw(win, "        `` ``       .oooooooooooooooooooo__,.Jooooooo__                     _roL   |ooo__oooooooooooooooooooooooooooooooooooooooooooooooooooo7,,     oor`    \n");
    wprintw(win, "                    ``.ooooooooooooooooooooo_ooooor```7|                   |r_``7_ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooro     ``      \n");
    wprintw(win, "                       `7ooooooooooooooooooooooooooorJooL                    ``ooooooooooooooo7o7oooor``ooooooooooooooooooooooooooooooooooooor`r             \n");
    wprintw(win, "                        |oooooooooooooooooooooooo''''                       ___oor`7oo/`oooor ._ `ooL  `oooooooooooooooooooooooooooooooooo``` Jo/            \n");
    wprintw(win, "                        `ooooooooooooooooooooor`                            oooo`  ||.Jr`oo.ooooooooor `oooooooooooooooooooooooooooor``oo,   .o`             \n");
    wprintw(win, "                         ``ooooooooooooooooooo`                             .o__ooooo``   __  _JoooooL__ooooooooooooooooooooooooooooor  ``__J``              \n");
    wprintw(win, "                           `oooooooooooor7oo`                              |ooooooooooo_Joo____ooooooooooooooooooooooooooooooooooooooo,  |r`                 \n");
    wprintw(win, "                             `roooooo`     oL_                           .ooooooooooooooooooooorooooooJ`7ooooooooooooooooooooooooooooo                       \n");
    wprintw(win, "        .,                    ```oooo     _ _o                          .ooooooooooooooooooooooL/ooooooooL_  `7ooooooooooooooooooor``J`                      \n");
    wprintw(win, "         `7,                     7ooo__Jo`   Jo , _                     |oooooooooooooooooooooooL`ooooooor      ooooor`  7ooooo|J|   |,                      \n");
    wprintw(win, "                                    ```ooJ_, `` ` `                     JooooooooooooooooooooooooL,ooo``        `ooo`    ``ooooo|    7L                      \n");
    wprintw(win, "                                         7o   __o____                   `7oooooooooooooooooooooooooL__J          7o|       ||`oo`   .`oo                     \n");
    wprintw(win, "                                           ``7oooooooL,.                  `oooooo7oooooooooooooooooooo`           `o|      `7,     ./ `o`                    \n");
    wprintw(win, "                                             Jooooooooooo|                         `oooooooooooooooor`             `      `oJo, ._Jo|  `.                    \n");
    wprintw(win, "                                            oooooooooooooL__,                      .oooooooooooooor`                        7o| 7ooo`L, | ._ ,               \n");
    wprintw(win, "                                            ooooooooooooooooooo_,                   `oooooooooooor                           7oL  `o oL````7`oooL._JL        \n");
    wprintw(win, "                                            |ooooooooooooooooooor                    |oooooooooooL                              `oo_____   ` ````Lo```7_     \n");
    wprintw(win, "                                             `ooooooooooooooooo`                     .oooooooooooo,  _,                              ```,.Jor .o ```   ``    \n");
    wprintw(win, ",                                             ``ooooooooooooooo                      oooooooooooo` .oo`                               _oooooo_Joo         L  \n");
    wprintw(win, "                                                |ooooooooooooo`                      `oooooooooL   Joo                            .__ooooooooooooo,      .,  \n");
    wprintw(win, "                                                |ooooooooor`                          7ooooooor`   oo`                            oooooooooooooooooL     ``  \n");
    wprintw(win, "                                                oooooooooo`                           `7oooooo`                                   `ooooooooooooooooo|        \n");
    wprintw(win, "                                                oooooooor`                              oor```                                     oooo`` ``ooooooor         \n");
    wprintw(win, "                                               Jooooooo                                 ``                                                   `oooo`         `\n");
    wprintw(win, "                                               oooor`                                                                                         ``oo          _\n");
    wprintw(win, "                                              .Joor`                                                                                            ``        _o`\n");
    wprintw(win, "                                              Jooo`                                                           .,                                          `` \n");
    wprintw(win, "                                              |ooL  `o`                                                                                                      \n");
    wprintw(win, "                                               ````                                                                                                          \n");
    wprintw(win, "                                                                                                                                                             \n");
    wprintw(win, "                                                  ._,``                                                ._,                   .,   ,         ,                \n");
    wprintw(win, "                                                _Jo_,                              , . ._    _______oooooooLoor .__JoooooooooooooooooooooooooooooL_____      \n");
    wprintw(win, "                       __._  ,    Joo_________Joooooo                   .__oooooooooooooooooooooooooooooooooooLJooooooooooooooooooooooooooooooooooooooooooo``\n");
    wprintw(win, "         .____Joooooooooooooooooooooooooooooooor```        ,    ._____Joooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo,   \n");
    wprintw(win, "       ``` ___Jooooooooooooooooooooooooooooooo______JL  Joooo_____Jooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo     \n");
    wprintw(win, "____L_______JooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooL_\n");
    wprintw(win, "ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
}
