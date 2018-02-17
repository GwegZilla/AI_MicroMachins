#pragma once
#include "CarController.h"
namespace aimm
{
	class PlayerCarController : public CarController
	{
	public:
		PlayerCarController(Car* l_poCar);
		~PlayerCarController();


		void Start();
		void Update();
		void Destroy();
	};

}