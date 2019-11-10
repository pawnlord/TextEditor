#include <iostream>
#include <stdio.h>
#include <vector>
#include <conio.h>
#include <stdlib.h>
#include <cstring>
#include <SFML/Graphics.hpp>
//character height
int char_size = 17;
int screen_width = 600;
// for making sure no characters are half off the screen
int rounder(int a, int b, int max = 100)
{
    for(int i = 0; i < 100; i++)
    {
        if(a * i < b && a * (i+1) > b)
            return a*i;
        if(a * i > b && a * (i-1) < b)
            return a*i;
    }
}
int screen_height = rounder(char_size, 500);
// width for buttons (for when figuring out when to add newlines
int button_width = 40;
// for making tue text;
void default_text(sf::Text& t, int x, int y, sf::Color c, int size)
{
    t.setCharacterSize(size);
    t.setStyle(sf::Text::Bold);
    t.setColor(c);
    t.setPosition(x,y);
}
sf::Font font;
// class for the buttons
class Button
{
    public:
    int x;
    int y;
    int resx;
    int resy;
    bool mstate = false;
    bool lmstate = false;
    sf::Color c;
    Button(int xl, int yl, sf::Color cl, int resxl, int resyl)
    {
        x = xl;
        y = yl;
        c = cl;
        resx = resxl;
        resy = resyl;
    }
    int move(int xl,int yl)
    {
        x+= xl;
        y+= yl;
    }
    void draw(sf::RenderWindow& window)
    {
        sf::RectangleShape r(sf::Vector2f(resx, resy));
        r.setPosition(x,y);
        r.setFillColor(c);
        lmstate = mstate;
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
           sf::Mouse::getPosition(window).x > x && sf::Mouse::getPosition(window).x < x + resx &&
           sf::Mouse::getPosition(window).y > y && sf::Mouse::getPosition(window).y < y + resy )
        {
            mstate = true;
        }
        else
            mstate = false;
        window.draw(r);
    }

};
// class for Text
class Text
{
    public:
    int x;
    int y;
    int size;
    sf::Color c;
    sf::Text t;
    std::string text_str;
    Text(int xl, int yl, sf::Color cl, int sizel, std::string text_str):
     t(text_str, font)
    {
        x = xl;
        y = yl;
        c = cl;
        size = sizel;
    }
    void change_text(std::string new_text)
    {
        t.setString(new_text);
    }
    int move(int xl,int yl)
    {
        x+= xl;
        y+= yl;
    }
    void draw(sf::RenderWindow& window)
    {
        default_text(t,x,y,c, size);
        window.draw(t);
    }
};
// for reading the file (so we can edit it)
void read_file(FILE* fp, std::vector<char>& buff)
{
	char c = 0;
	int i = 0;
	// repeat until c is EOF
	while(c != EOF )
	{
		c = fgetc((FILE*)fp);
		//printf("%c\n", c);
		if(c == EOF)
        {
            break;
        }
		buff.push_back(c);
		i++;
	}
}
int main(int argc, const char* argv[])
{
    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "SFML works!");
    //font
    font.loadFromFile("F:\\CPP\\Programs\\GraphicalTextEditor\\bin\\Release\\UbuntuMono-B.ttf");
    //slower framerate (less button presses it)
    window.setFramerateLimit(30);
    //check if we have file
    if(argc <= 1)
    {
        printf("NEEEED FIIILEE REEEEEEEEEEEEEEEEEEEEEEEEEEEEE");
        return 1;
    }
    FILE* fp;
    // open file in read and write file
    fp = fopen(argv[1], "r+");
    // where we store the text (for printing text and rewriting file)
    char buff[1024];
    // vector since it is easier to edit than an array
    std::vector<char> text;
    read_file(fp, text);
    char c = 0;
    // closing file (will need to update it with different open type)
    fclose(fp);
    bool b = 0;
    // for the cursor
    int char_on = text.size()-1;
    // GUI elements
    Text t(0,0, sf::Color::Black, char_size, "");
    Button save(510,10,sf::Color::Green, 80, 80);
    Button quit(510,110,sf::Color::Red, 80, 80);
    Button sq(510,210,sf::Color::Yellow, 80, 80);

    Text ts(520,20, sf::Color::Black, 30, "Save");
    Text tq(520,120, sf::Color::Black, 30, "Quit");
    Text tsq(520,205, sf::Color::Black, 30, "Save &\nQuit");
    // variables for for loops
    int a_chars = 0;
    int line_char_num = 0;
            int i ;
    bool was_change = true;
    bool arrow_pressed = false;
    bool hasMoved = false;
    bool hasRMoved = false;
    // main loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            //check for any buttons for text
            if (event.type == sf::Event::TextEntered)
            {
                was_change = true;
                t.y = 0;
                c= (char)event.text.unicode;
                if(c=='*')
                    break;
                system("cls");
                if(c == 8 && text.size() > 0)
                {
                    // erase the 6th element

                    text.erase (text.begin()+(char_on));
                    char_on--;

                }
                // Enter Button Case
                else if(c == 13)
                {
                    text.insert(text.begin() + char_on+1, '\n');

                    char_on++;
                }
                else{
                    text.insert(text.begin() + char_on+1, c);
                    char_on++;
                }
            }
            // change size of text
            if(event.type == sf::Event::MouseWheelScrolled)
            {
                was_change = true;
                if(event.mouseWheelScroll.delta > 0)
                    char_size += 5;
                if(event.mouseWheelScroll.delta < 0 && char_size - 5 > 0)
                    char_size -= 5;
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && char_on > 0 && hasMoved == false)
        {
            hasMoved = true;
            char_on--;
            was_change = true;
            arrow_pressed = true;
        }
        if(hasMoved == true && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            hasMoved = false;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
           (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) ||sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))&& hasMoved == false
            && char_on > 0)
        {
            hasMoved = true;
            char_on -= 9;
            if(char_on < 0)
                char_on = 0;
            was_change = true;
            arrow_pressed = true;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && char_on < text.size() - 1 && hasRMoved == false)
        {
            hasRMoved = true;
            char_on++;
            was_change = true;
            arrow_pressed = true;
        }
        if(hasRMoved == true && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            hasRMoved = false;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)&&
           (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) ||sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) && hasRMoved == false
             && char_on < text.size() - 1)
        {
            char_on += 9;
            if(char_on > text.size() - 1)
                char_on = text.size() - 1;
            was_change = true;
            arrow_pressed = true;
        }
        if(screen_width != window.getSize().x)
        {
            screen_width = window.getSize().x;
            was_change = true;
        }
        if(screen_height != window.getSize().y)
        {
            screen_height = window.getSize().y;
            was_change = true;
        }
        c = 0;
        if(was_change)
        {
            int k = 0;
            int l = 0;
                int line_char_num = 0;
                t.y = 0;
                for( int j = 0; j < char_on;j++)
                {
                    line_char_num++;
                    if(j >= text.size())
                        break;
                    if(text[j] == '\n' ||line_char_num * char_size/1.5 > screen_width - button_width)
                    {
                        k++;
                        line_char_num = 0;
                    }
                    if(k * char_size > screen_height/2)
                    {
                        l++;
                        k--;
                    }
                }
                t.y -= l * char_size;
                int line_num = 0;
                memset(buff, 0, sizeof buff);
                        int a_chars = 0;

                line_char_num = 0;
                for( i = 0; i < text.size();i++)
                {
                    buff[i + a_chars] = text[i];
                    if(i == char_on)
                    {
                        a_chars++;
                        buff[i + a_chars] = '|';
                    }
                    line_char_num ++;
                    if(line_char_num * char_size/1.5 > screen_width - button_width)
                    {
                        a_chars++;
                        for( int j = text.size() + a_chars; j >= 0;j--)
                        {
                            if(buff[j] == ' ')
                            {
                                char new_buff[1024];
                                strcpy(new_buff, buff);
                                new_buff[j+1] = '\n';
                                for(int k = text.size() + a_chars - ((text.size() + a_chars) - (j+1));
                                            k  < text.size() + a_chars; k++)
                                    new_buff[k+1] = buff[k];
                                strcpy(buff, new_buff);
                                break;
                            }
                        }
                        line_char_num = 0;
                        line_num++;
                    }
                    if(text[i] == '\n')
                        line_char_num = 0;
                }
                //i++;
                i+=a_chars;
        }
        arrow_pressed = false;
        was_change = false;
        t.size = char_size;
        t.change_text(buff);
        window.clear(sf::Color::White);
        save.draw(window);
        quit.draw(window);
        sq.draw(window);
        t.draw(window);
        ts.draw(window);
        tq.draw(window);
        tsq.draw(window);
        if(save.mstate && !save.lmstate)
        {
            char sbuff[1024];
            fp = fopen(argv[1], "w+");
            for(int i = 0; i < text.size();i++)
            {
                sbuff[i] = text[i];

            }
            fputs( sbuff , fp );
            fclose(fp);
        }
        if(quit.mstate && !quit.lmstate)
            return 0;
        if(sq.mstate && !quit.lmstate)
            break;
        window.display();
    }

    fp = fopen(argv[1], "w+");
            memset(buff, 0, sizeof buff);
    for(int i = 0; i < text.size();i++)
    {
        buff[i] = text[i];

    }
    fputs( buff , fp );
    fclose(fp);
    return 0;
}
