#pragma once

/*#ifdef MYLIBRARY_EXPORTS
#define MYLIBRARY_API __declspec(dllexport)
#else
#define MYLIBRARY_API __declspec(dllimport)
#endif*/

namespace MyLibrary {

	void swap(int& a, int& b);

	/*FVector GetRandomPosition1() {
		return FVector(FMath::RandRange(-12700, 12500), FMath::RandRange(-12700, 12500), 100);
	}*/

}