#include "FontFileEnumerator.h"
#include "Factory.h"

namespace MS { namespace Internal { namespace Text { namespace TextInterface
{
    FontFileEnumerator::FontFileEnumerator(
                                          IEnumerable<IFontSource^>^ fontSourceCollection,
                                          FontFileLoader^            fontFileLoader,
                                          IDWriteFactory*            factory
                                          )
    {
        _fontSourceCollectionEnumerator = fontSourceCollection->GetEnumerator();                
        _fontFileLoader                 = fontFileLoader;
        factory->AddRef();
        _factory                        = factory;
    }

    /// <SecurityNote>
    /// Critical - Asserts unmanaged code permissions to call Marshal.* However the call to Marshal is safe
    ///            because it is called with trusted inputs.
    /// </SecurityNote>
    [ComVisible(true)]
    [SecurityCritical]
    [SecurityPermission(SecurityAction::Assert, UnmanagedCode=true)]
    HRESULT FontFileEnumerator::MoveNext(
                                        __out bool% hasCurrentFile
                                        )
    {
        HRESULT hr = S_OK;
        try
        {
            hasCurrentFile = _fontSourceCollectionEnumerator->MoveNext();
        }
        catch(System::Exception^ exception)
        {
            hr = System::Runtime::InteropServices::Marshal::GetHRForException(exception);
        }

        return hr;
    }

    HRESULT FontFileEnumerator::GetCurrentFontFile(
                                                  __out IDWriteFontFile** fontFile
                                                  ) 
    {
        if (fontFile == 0)
        {
            return E_INVALIDARG;
        }

        return Factory::CreateFontFile(
                                      _factory,
                                      _fontFileLoader,
                                      _fontSourceCollectionEnumerator->Current->Uri,
                                      fontFile
                                      );
    }
}}}}//MS::Internal::Text::TextInterface
