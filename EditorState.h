#pragma once

#include "GameState.h"

class EditorState : public GameState
{
public:
	void init(Game* game);
	void cleanup();

	void pause();
	void resume();

	void update(double dt);
	void draw();
	void msgProc(UINT msg, WPARAM wParam, LPARAM lParam);

	void onLostDevice();
	void onResetDevice();

	static EditorState* Instance() {
		return &mEditorState;
	}
protected:
	EditorState() {};
private:
	static EditorState mEditorState;

	World*		mWorld;
};	// Class