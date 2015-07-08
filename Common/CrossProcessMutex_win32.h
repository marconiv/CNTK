#pragma once

class CrossProcessMutex
{
    // no-copying
    CrossProcessMutex(const CrossProcessMutex&);
    void operator=(const CrossProcessMutex&);
    
    std::string m_name; // lock name
    HANDLE m_handle;
public:
    CrossProcessMutex(const std::string& name)
        :m_handle(NULL),
         m_name("Global\\" + name)
    {
    }

    bool Acquire(bool wait)
    {
        assert (m_handle == NULL);
        m_handle = ::CreateMutexA(NULL/*security attr*/, FALSE/*bInitialOwner*/, m_name.c_str());
        if (m_handle == NULL)
        {
            return false;
        }
        
        if (::WaitForSingleObject(m_handle, wait ? INFINITE : 0) != WAIT_OBJECT_0)
        {
            ::CloseHandle(m_handle);
            m_handle = NULL;
            return false;
        }
        
        return true;
    }
    
    void Release()
    {
        assert (m_handle != NULL);
        ::CloseHandle(m_handle);
        m_handle = NULL;
    }
    
    ~CrossProcessMutex()
    {
        if (m_handle != NULL)
        {
            Release();
        }
    }
};