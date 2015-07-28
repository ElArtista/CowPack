/*********************************************************************************************************************/
/*                                                  /===-_---~~~~~~~~~------____                                     */
/*                                                 |===-~___                _,-'                                     */
/*                  -==\\                         `//~\\   ~~~~`---.___.-~~                                          */
/*              ______-==|                         | |  \\           _-~`                                            */
/*        __--~~~  ,-/-==\\                        | |   `\        ,'                                                */
/*     _-~       /'    |  \\                      / /      \      /                                                  */
/*   .'        /       |   \\                   /' /        \   /'                                                   */
/*  /  ____  /         |    \`\.__/-~~ ~ \ _ _/'  /          \/'                                                     */
/* /-'~    ~~~~~---__  |     ~-/~         ( )   /'        _--~`                                                      */
/*                   \_|      /        _)   ;  ),   __--~~                                                           */
/*                     '~~--_/      _-~/-  / \   '-~ \                                                               */
/*                    {\__--_/}    / \\_>- )<__\      \                                                              */
/*                    /'   (_/  _-~  | |__>--<__|      |                                                             */
/*                   |0  0 _/) )-~     | |__>--<__|     |                                                            */
/*                   / /~ ,_/       / /__>---<__/      |                                                             */
/*                  o o _//        /-~_>---<__-~      /                                                              */
/*                  (^(~          /~_>---<__-      _-~                                                               */
/*                 ,/|           /__>--<__/     _-~                                                                  */
/*              ,//('(          |__>--<__|     /                  .----_                                             */
/*             ( ( '))          |__>--<__|    |                 /' _---_~\                                           */
/*          `-)) )) (           |__>--<__|    |               /'  /     ~\`\                                         */
/*         ,/,'//( (             \__>--<__\    \            /'  //        ||                                         */
/*       ,( ( ((, ))              ~-__>--<_~-_  ~--____---~' _/'/        /'                                          */
/*     `~/  )` ) ,/|                 ~-_~>--<_/-__       __-~ _/                                                     */
/*   ._-~//( )/ )) `                    ~~-'_/_/ /~~~~~~~__--~                                                       */
/*    ;'( ')/ ,)(                              ~~~~~~~~~~                                                            */
/*   ' ') '( (/                                                                                                      */
/*     '   '  `                                                                                                      */
/*********************************************************************************************************************/
#include <iostream>
#include <string>
#include <tclap/CmdLine.h>
#include <dirent.h>

std::string basename(const std::string& pathName)
{
    return std::string( 
        std::find_if(
            pathName.rbegin(),
            pathName.rend(),
            [](char ch) { return ch == '\\' || ch == '/'; }
        ).base(),
        pathName.end()
    );
}

/*
 * List files and directories within a directory.
 */
void list_directory(const char* dirname)
{
    DIR* dir;
    struct dirent* ent;

    /* Open directory stream */
    dir = opendir(dirname);
    if (dir != NULL)
    {
        /* Print all files and directories within the directory */
        while ((ent = readdir(dir)) != NULL)
        {
            switch (ent->d_type)
            {
                case DT_REG:
                    printf ("%s\n", ent->d_name);
                    break;

                case DT_DIR:
                    printf ("%s/\n", ent->d_name);
                    break;

                case DT_LNK:
                    printf ("%s@\n", ent->d_name);
                    break;

                default:
                    printf ("%s*\n", ent->d_name);
            }
        }

        closedir(dir);
    }
    else
    {
        /* Could not open directory */
        printf ("Cannot open directory %s\n", dirname);
        exit (EXIT_FAILURE);
    }
}

class CMLOutput : public TCLAP::StdOutput
{
    public:
        virtual void usage(TCLAP::CmdLineInterface& c)
        {
            std::string progName = basename(c.getProgramName());
            std::string description = c.getMessage();
            std::cout << "Usage: " << progName << " "
                << "[OPTIONS...]" << " " 
                << "[FILES]..."
                << std::endl;
            std::cout << description << std::endl;
            //_shortUsage(c, std::cout);
            std::cout << std::endl << "Options:" << std::endl << std::endl;
            _longUsage(c, std::cout);
            std::cout << std::endl; 
        }

        virtual void version(TCLAP::CmdLineInterface& c)
        {
            std::string progName = basename(c.getProgramName());
            std::string version = c.getVersion();
            std::cout << progName << " " << version << std::endl;
        }

        virtual void failure(TCLAP::CmdLineInterface& c, TCLAP::ArgException& e)
        {
            std::string progName = basename(c.getProgramName());
            std::cerr << progName << ": " << e.what() << std::endl;
            std::cerr << "Try `" << progName << " --help' for more information." << std::endl;
            throw TCLAP::ExitException(-1);
        }
};

namespace TCLAP
{
    class RequiredDependentArgException : public ArgException
    {
        public:
            RequiredDependentArgException(
                    const TCLAP::Arg& parentArg,
                    const TCLAP::Arg& requiredArg)
                : ArgException(
                        std::string( "Required argument ") +
                        requiredArg.toString() +
                        std::string(" missing when the ") +
                        parentArg.toString() +
                        std::string(" flag is specified."),
                        requiredArg.toString())
        {}
    };
}

int main(int argc, char* argv[])
{
    try
    {
        // Define the parser
        TCLAP::CmdLine cmd("A command line util to create, view and extract CowPack files", ' ', "0.0.1");
        cmd.setExceptionHandling(false);

        // Set the custom output
        CMLOutput output;
        cmd.setOutput(&output);

        // Define the verbose switch
        TCLAP::SwitchArg verboseFlag("v", "verbose", "Verbosely list files processed");
        cmd.add(verboseFlag);

        // Define the main switches
        TCLAP::SwitchArg listFlag("t", "list", "Lists the contents of the archive");
        TCLAP::SwitchArg createFlag("c", "create", "Creates a new archive");
        TCLAP::SwitchArg extractFlag("x", "extract", "Extracts files from archive");

        // Xor the main switches
        std::vector<TCLAP::Arg*> mainSw;
        mainSw.push_back(&listFlag);
        mainSw.push_back(&createFlag);
        mainSw.push_back(&extractFlag);
        cmd.xorAdd(mainSw);

        // Define the cowfile switch
        TCLAP::ValueArg<std::string> fileArg("f", "file", "Uses given archive file", true, "", "ARCHIVE");
        cmd.add(fileArg);

        // Define the contents switch
        TCLAP::UnlabeledMultiArg<std::string> contents("contents",
                "The files and directories to be added to the archive", false, "FILES");
        cmd.add(contents);

        // Parse the args.
        cmd.parse(argc, argv);

        // Analyse them after parsing
        if (verboseFlag.isSet())
            std::cout << "Verbose Mode ON." << std::endl;

        if (listFlag.isSet())
            std::cout << "List Mode ON." << std::endl;
        else if (createFlag.isSet())
            std::cout << "Create Mode ON." << std::endl;
        else if (extractFlag.isSet())
            std::cout << "Extract Mode ON." << std::endl;

        if (createFlag.isSet() && !contents.isSet())
            throw(TCLAP::RequiredDependentArgException(createFlag, contents));

        if ((listFlag.isSet() || extractFlag.isSet()) && contents.isSet())
            std::cout << "Warning: unnecessary contents are being ignored" << std::endl;

        std::string cowfile = fileArg.getValue();
        std::cout << "Cowfile value: " << cowfile << std::endl;

        if (contents.isSet())
        {
            std::vector<std::string> contentValues = contents.getValue();
            std::cout << "Content values: " << std::endl;
            for (const auto& c : contentValues)
                std::cout << "\t" << c << std::endl;
        }
    }
    catch (TCLAP::ArgException& e)
    {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        return -1;
    }
    catch (TCLAP::ExitException& e)
    {
        return e.getExitStatus();
    }

    return 0;
}

