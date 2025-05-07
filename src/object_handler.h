#ifndef _INCLUDE_OBJECT_HANDLER_H
#define _INCLUDE_OBJECT_HANDLER_H

#include "smsdk_ext.h"

class DiscordObject
{
};

template <class T = DiscordObject> class DiscordObjectHandler : public IHandleTypeDispatch
{
public:
    HandleType_t HandleType;

    virtual ~DiscordObjectHandler() = default;
    
    Handle_t CreateHandle(T* object, const HandleSecurity* sec, HandleError* err)
    {
        Handle_t handle = handlesys->CreateHandleEx(HandleType, object, sec, nullptr, err);

        return handle;
    }

    T* ReadHandle(Handle_t handle, const HandleSecurity* sec, HandleError* err)
    {
        T* obj;
        if ((*err = handlesys->ReadHandle(handle, HandleType, sec, (void**)&obj)) != HandleError_None)
        {
            return nullptr;
        }

        return obj;
    }

    T* ReadHandle(Handle_t handle)
    {
        HandleError err;
        HandleSecurity sec(myself->GetIdentity(), myself->GetIdentity());
        return ReadHandle(handle, &sec, &err);
    }

    void OnHandleDestroy(HandleType_t type, void* object) override
    {
        T* obj = (T*)object;
        delete obj;
    }
};

#endif //_INCLUDE_OBJECT_HANDLER_H
