#include "Attribute.h"

namespace HazardScript::Utils 
{
	const char* ProgressToString(const Progress& progress) {
		switch (progress)
		{
		case Progress::Done:		return "Done";
		case Progress::Awaiting:	return "Awaiting";
		case Progress::InProgress:	return "InProgress";
		case Progress::Bug:			return "Bug";
		}
		return "Unknown";
	}
}