
import sys, os, subprocess, shutil

TOOLSPATH = os.path.dirname(sys.argv[0])
ARGUMENTS = sys.argv[1:]

# -h, --help, -help, --h {compile|...} = help for action
# -k {keyfilepath} = the path to the keyfile
# -b = binarize configs
# -T {pathTo} = to location
# -F {pathFrom}, -f {files path from (..\*)} = from location
# -I {} = wildcards (seperated by comma (,)) to include while compiling
# -A {path to addon builder} = "
# -S {path to DSUtils} = "
# -p {prefix (variable {folder} available)} = sets the .pbo prefix; {folder} return .pbo folder basename
# -N "lilc_{name}" = sets the file prefix

ARGPARAMS = [
    ["-h", 0],
    ["--help", 0],
    ["-help", 0],
    ["--h", 0],
    ["-k", 1],
    ["-b", 0],
    ["-T", 1],
    ["-F", 1],
    ["-I", 1],
    ["-A", 1],
    ["-S", 1],
    ["-L", 1],
    ["-p", 1],
    ["-N", 1]
]
ARGSVALUES = []

AVARGS = []
for arg in ARGPARAMS:
    AVARGS.append(arg[0])

def getArgValue(argument):
    for arg in ARGSVALUES:
        if arg[0] == argument:
            return arg[1:]
    return []

def argInList(argument):
    for arg in ARGSVALUES:
        if arg[0] == argument:
            return True
    return False

for i in range(0, len(ARGUMENTS)):
    if ARGUMENTS[i] in AVARGS:
        args = [
            ARGUMENTS[i]
        ]
        a = ARGPARAMS[AVARGS.index(ARGUMENTS[i])][1]

        if a > 0:
            for b in range(0, a):
                if i + 1 + b >= len(ARGUMENTS):
                    args.append("")
                else:
                    args.append(ARGUMENTS[(i + 1)])   
        ARGSVALUES.append(args)

if argInList("-h") or argInList("--help") or argInList("-help") or argInList("--h"):
    print("LiveInLife build tools help:")
    print("\t-h, --h, -help, --help = help for action")
    print("\t-k {keyfilepath} = the path to the keyfile")
    print("\t-b = binarize configs")
    print("\t-T {pathTo} = to location")
    print("\t-F {pathFrom (use \* at the end to select all folders behind current path)} = from location")
    print("\t-I {} = wildcards (seperated by comma (,)) to include while compiling")
    print("\t-A {path to addon builder} = path of your installed addon builder")
    print("\t-S {path to DSUtils} = path to your installed DSUtils program")
    print("\t-p {prefix (variable {folder} available)} = sets the .pbo prefix; {folder} return .pbo folder basename")
    print("\t-L {path to logfile} = path to the logfile")

    sys.exit(1)

KEYFILE = ""
kVal = getArgValue("-k")
if len(kVal) >= 1:
    KEYFILE = kVal[0]
del kVal

BINARIZE = False
if argInList("-b"):
    BINARIZE = True

PATHTO = ""
pTVal = getArgValue("-T")
if len(pTVal) >= 1:
    PATHTO = pTVal[0]
del pTVal

PATHSFROM = []
pFVal = getArgValue("-F")

if len(pFVal) >= 1:
    p = pFVal[0]
    bN = os.path.basename(p)

    if (bN == "*"):
        p = os.path.dirname(p)
        for folder in os.listdir(p):
            absP = os.path.join(p, folder)
            if os.path.isdir(absP):
                PATHSFROM.append(absP)
    else:
        PATHSFROM.append(p)
del pFVal

INCLUDE = ""
incVal = getArgValue("-I")
if len(incVal) >= 1:
    INCLUDE = incVal[0]
del incVal

PATHADDONBUILDER = ""
pABVal = getArgValue("-A")
if len(pABVal) >= 1:
    PATHADDONBUILDER = pABVal[0]
del pABVal

PATHDSUTILS = ""
pDVal = getArgValue("-S")
if len(pDVal) >= 1:
    PATHDSUTILS = pDVal[0]
del pDVal

PREFIX = ""
pVal = getArgValue("-p")
if len(pVal) >= 1:
    PREFIX = pVal[0]
del pVal

PATHLOGFILE = os.path.join(TOOLSPATH, "compile.log")
lVal = getArgValue("-L")
if len(lVal) >= 1:
    PATHLOGFILE = lVal[0]
del lVal

FILENAMETEMPLATE = "{name}"
nVal = getArgValue("-N")
if len(nVal) >= 1:
    FILENAMETEMPLATE = nVal[0]
del nVal

lFile = open(PATHLOGFILE, 'w+')
lFile.seek(0)
lFile.truncate()

iFilePath = os.path.join(TOOLSPATH, "inc.tmp")

TEMPPATH = os.path.abspath(os.path.join(TOOLSPATH, "tmp"))

if not os.path.isdir(TEMPPATH):
    os.makedirs(TEMPPATH)

if INCLUDE != "":
    iFile = open(iFilePath, 'w+')
    iFile.seek(0)
    iFile.truncate()
    iFile.write(INCLUDE)
    iFile.close()

for path in PATHSFROM:
    fName = os.path.basename(path)
    path = os.path.abspath(path)

    templateName = FILENAMETEMPLATE

    fNameTemplateFile = os.path.join(path, "$NAMETEMPLATE$")
    if os.path.isfile(fNameTemplateFile):
        f = open(fNameTemplateFile)
        templateName = f.readline()
        f.close()

    tmpPath = os.path.abspath(os.path.join(TEMPPATH, templateName.replace("{name}", fName)))
    fPrefixFile = os.path.join(tmpPath, "$PREFIX$")
    prefix = PREFIX.replace("{folder}", fName)

    if os.path.isdir(tmpPath):
        shutil.rmtree(tmpPath)

    shutil.copytree(path, tmpPath)

    if os.path.isfile(fPrefixFile):
        f = open(fPrefixFile)
        prefix = f.readline()
        f.close()

    print("----\nNew package: %s\n----\n" % prefix)
    lFile.write("----\nNew package: %s\n----\n" % prefix)
    params = [
        ('"%s"' % os.path.abspath(PATHADDONBUILDER)),
        ('"%s"' % tmpPath),
        ('"%s"' % os.path.abspath(PATHTO))
    ]

    if prefix != "":
        prefix = ('-prefix="%s"' % prefix)
        params.append(prefix)

    sF = ""
    if KEYFILE != "":
        sF = ('-sign="%s"' % os.path.abspath(KEYFILE))
        params.append(sF)

    dSF = ""
    if PATHDSUTILS != "":
        dSF = ('-dssignfile="%s"' % os.path.abspath(PATHDSUTILS))
        params.append(dSF)

    inc = ""
    if INCLUDE != "":
        inc = ('-include="%s"' % iFilePath)
        params.append(inc)

    binar = ""
    if not BINARIZE:
        binar = "-packonly"
        params.append(binar)

    params.append('-clear')

    cmd = " ".join(params)
    print(cmd)

    strBuf = subprocess.check_output(cmd)
    encStrBuf = strBuf.decode(sys.stdout.encoding)
    print(encStrBuf)
    
    if os.path.isdir(tmpPath):
        shutil.rmtree(tmpPath)
    lFile.write(encStrBuf)
lFile.close()
if os.path.isfile(iFilePath):
    os.remove(iFilePath)


# test.py compile --h
# -h, --help, -help, --h {compile|...} = help for action
# -k {keyfilepath} = the path to the keyfile
# -b = binarize configs
# -T {pathTo} = to location
# -F {pathFrom} = from location
# -I {} = wildcards (seperated by comma (,)) to include while compiling
# -A {path to addon builder} = "
# -S {path to DSUtils} = "
# -p {prefix (variable {folder} available)} = sets the .pbo prefix; {folder} return .pbo folder basename
# -L {path to logfile} = path to the logfile
