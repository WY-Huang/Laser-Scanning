#include "my_params.h"

My_params *My_params::Get()
{
    static My_params fun;
    return &fun;
}

My_params::My_params()
{
    cam = cam->Get();
}

My_params::~My_params()
{

}
