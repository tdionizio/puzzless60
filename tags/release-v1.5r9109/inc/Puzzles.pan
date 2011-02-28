
#ifndef __PUZZLES_PAN__
#define __PUZZLES_PAN__

/** Puzzles application panic codes */
enum TPuzzlesPanics
	{
	EPuzzlesUi = 1
	// add further panics here
	};

inline void Panic(TPuzzlesPanics aReason)
	{
	_LIT(applicationName, "Puzzles");
	User::Panic(applicationName, aReason);
	}

#endif // __PUZZLES_PAN__
