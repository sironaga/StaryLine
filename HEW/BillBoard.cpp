#include "BillBoard.h"

void InitBillBoard()
{

}

void SetBillBoard(Camera* camera, DirectX::XMMATRIX world)
{
	DirectX::XMMATRIX view_mat = camera->GetView();
	view_mat = DirectX::XMMatrixInverse(nullptr, view_mat);
	
}
