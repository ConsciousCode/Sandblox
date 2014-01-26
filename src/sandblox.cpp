/*
This file is part of Sandblox.

	Sandblox is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Sandblox is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Sandblox. If not, see <http://www.gnu.org/licenses/>.
*/

#include "include.hpp"

int main(int argc,char* argv[]){
	Game* g=init();
	printf("Survived initializing the game!\n");

	Block b("sand","resources/sand.jpg");
	g->scene->add_block(&b);

	g->run();

	deinit(g);

	return 0;
}