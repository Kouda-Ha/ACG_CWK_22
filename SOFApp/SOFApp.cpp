// SOFApp.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "MainScene.h"

int main()
{
	MainScene *app = new MainScene();
	SOF::Window::CreateWindow(1024, 768, "Owen Clifford Park (MMU ID 19095713)", app);
	SOF::Window::Run();

    return 0;
}