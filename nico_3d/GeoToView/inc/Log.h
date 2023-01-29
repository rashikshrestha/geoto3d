// Copyright NVIDIA Corporation 2009-2010
// TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED
// *AS IS* AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS
// OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS
// BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES
// WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
// BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS)
// ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS
// BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES

#include <stdarg.h>

#pragma once

inline void LogMessage( const char * format, ... )
{
  va_list valist;
  va_start( valist, format );

  fprintf(stderr,"LOG1!!!!!!!\n");
  //GetApp()->log( format, valist, LogWidget::LOG_MESSAGE );
  va_end( valist );
}

inline void LogWarning( const char * format, ... )
{
  va_list valist;
  va_start( valist, format );

  fprintf(stderr,"LOG2!!!!!!!\n");
  //GetApp()->log( format, valist, LogWidget::LOG_WARNING );
  va_end( valist );
}

inline void LogError( const char * format, ... )
{
  va_list valist;
  va_start( valist, format );

  fprintf(stderr,"LOG3!!!!!!!\n");
  //GetApp()->log( format, valist, LogWidget::LOG_ERROR );
  va_end( valist );
}

