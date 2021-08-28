//
// Created by gvisan on 17.08.2021.
//

#ifndef CHESSENGINE_UCI_H
#define CHESSENGINE_UCI_H

#include<sstream>
#include<iostream>
#include<string>
#include"timeman.h"
#include"history.h"
#include"search.h"

namespace engine {
    /*This class is responsible with communicating with the GUI using the UCI protocol.*/
    /*UCI protocol specifications:http://wbec-ridderkerk.nl/html/UCIProtocol.html.*/
    class UCI {
    private:
        /*The internal board.*/
        Board board;

        /*The search class.*/
        Search search;

        /*Move generation class, used to find the move received in string form from the GUI.*/
        MoveGen move_gen;

        /*Sends the engine info and options to the gui when we receive the "uci" command.*/
        void send_info() const;

        /*Process the command that was received from the GUI.*/
        void process(std::stringstream &command);

        /*Process the position given by the "position" command.*/
        void process_position(std::stringstream &position);

        /*Process the parameters given by the "go" command and start the search.*/
        void process_search(std::stringstream &parameters);

        /*Process the option values given by the "setoption" command.*/
        void process_option(std::stringstream &option);

        /*Finds the move in the current position that has the given string representation.
         * Return an empty move if it was not found.*/
        Move find_move(const std::string &move);

        /*This flag is set when we receive a quit command.*/
        bool stop_uci;

    public:
        explicit UCI();

        /*Starts listening for commands from the GUI.*/
        void start();

    };
}


#endif //CHESSENGINE_UCI_H
