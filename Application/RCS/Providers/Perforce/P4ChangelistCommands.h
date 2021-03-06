#pragma once

#include "P4Command.h"

namespace Helium
{
    namespace Perforce
    {
        class GetChangelistsCommand : public Command
        {
        public:
            GetChangelistsCommand( Provider* provider, RCS::V_Changeset* changesets )
                : Command ( provider, TXT( "changes" ) )
                , m_Changesets( changesets )
            {
            }

            virtual void OutputStat( StrDict* dict ) HELIUM_OVERRIDE;
            virtual void Run() HELIUM_OVERRIDE;

        protected:
            RCS::V_Changeset* m_Changesets;
        };

        class CreateChangelistCommand : public Command
        {
        public:
            CreateChangelistCommand( Provider* provider, RCS::Changeset* changeset )
                : Command ( provider, TXT( "change" ) )
                , m_Changeset( changeset )
            {
            }

            virtual void Run() HELIUM_OVERRIDE;
            virtual void InputData( StrBuf *buf, Error *e );
            virtual void OutputInfo( char level, const char* data ) HELIUM_OVERRIDE;

        protected:
            RCS::Changeset* m_Changeset;
        };

        class DeleteChangelistCommand : public Command
        {
        public:
            DeleteChangelistCommand( Provider* provider )
                : Command ( provider, TXT( "change" ) )
                , m_Changelist ( RCS::InvalidChangesetId )
            {
            }

            u64 GetChangelist()
            {
                return m_Changelist;
            }

        protected:
            u64 m_Changelist;
        };

        class OpenedCommand : public Command
        {
        public:
            OpenedCommand( Provider* provider, RCS::V_File* files )
                : Command ( provider, TXT( "opened" ) )
                , m_FileList( files )
            {
            }

            virtual void OutputStat( StrDict* dict ) HELIUM_OVERRIDE;

            RCS::V_File* m_FileList;
        };

        class SubmitCommand : public Command
        {
        public:
            SubmitCommand( Provider* provider, RCS::Changeset* changeset = NULL )
                : Command ( provider, TXT( "submit" ) )
                , m_Changeset( changeset )
            {
            }

            virtual void OutputStat( StrDict* dict );

            RCS::Changeset* m_Changeset;
        };

        class RevertCommand : public Command
        {
        public:
            RevertCommand( Provider* provider, RCS::File* file = NULL )
                : Command ( provider, TXT( "revert" ) )
                , m_File ( file )
            {
            }

            virtual void OutputStat( StrDict* dict );

        protected:
            RCS::File* m_File;
        };
    }
}