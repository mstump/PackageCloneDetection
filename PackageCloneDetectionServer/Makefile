BINDIR=bin
SRCDIR=src
CPP=g++
CPPFLAGS=-O3 -Wall -g -I $(SRCDIR)

ALLBINS=	$(BINDIR)/MatchDistro \
		$(BINDIR)/Find2Cliques \
		$(BINDIR)/BasePackage \
		$(BINDIR)/MakePackageVectors \
		$(BINDIR)/FindEmbeddedPackages \
		$(BINDIR)/CountDuplicateFilenames \
		$(BINDIR)/ReduceFilenameFeatures \
		$(BINDIR)/UseTheseFeatures \
		$(BINDIR)/SSDeepProcessingForDEB \
		$(BINDIR)/FindClique \
		$(BINDIR)/FindEmbeddedFromClique \
		$(BINDIR)/FindMissingCVEFromClique \
		$(BINDIR)/CullMatchesByCount \
		$(BINDIR)/CheckEmbedded \
		$(BINDIR)/StemMatches

all:	MatchDistro \
	Find2Cliques \
	BasePackage \
	StemMatches \
	CheckEmbedded \
	MakePackageVectors \
	FindEmbeddedPackages \
	CountDuplicateFilenames \
	ReduceFilenameFeatures \
	UseTheseFeatures \
	SSDeepProcessingForDEB \
	FindEmbeddedFromClique \
	FindMissingCVEFromClique \
	CullMatchesByCount
#	FindClique

Find2Cliques: $(SRCDIR)/Find2Cliques.cpp
	$(CPP) $(CPPFLAGS) $< -o $(BINDIR)/$@

MatchDistro: $(SRCDIR)/MatchDistro.cpp
	$(CPP) $(CPPFLAGS) $< -o $(BINDIR)/$@

BasePackage: $(SRCDIR)/BasePackage.cpp
	$(CPP) $(CPPFLAGS) $< -o $(BINDIR)/$@

MakePackageVectors: $(SRCDIR)/MakePackageVectors.cpp
	$(CPP) $(CPPFLAGS) $< -o $(BINDIR)/$@

FindEmbeddedPackages: $(SRCDIR)/FindEmbeddedPackages.cpp
	$(CPP) $(CPPFLAGS) $< -o $(BINDIR)/$@

CountDuplicateFilenames: $(SRCDIR)/CountDuplicateFilenames.cpp
	$(CPP) $(CPPFLAGS) $< -o $(BINDIR)/$@

ReduceFilenameFeatures: $(SRCDIR)/ReduceFilenameFeatures.cpp
	$(CPP) $(CPPFLAGS) $< -o $(BINDIR)/$@

UseTheseFeatures: $(SRCDIR)/UseTheseFeatures.cpp
	$(CPP) $(CPPFLAGS) $< -o $(BINDIR)/$@

SSDeepProcessingForDEB: $(SRCDIR)/SSDeepProcessingForDEB.cpp
	$(CPP) $(CPPFLAGS) $< -o $(BINDIR)/$@ -lfuzzy

FindClique: $(SRCDIR)/FindClique.cpp
	$(CPP) $(CPPFLAGS) $< -o $(BINDIR)/$@ - scotch

FindEmbeddedFromClique: $(SRCDIR)/FindEmbeddedFromClique.cpp $(SRCDIR)/Kruskal.cpp $(SRCDIR)/Prims.cpp
	$(CPP) $(CPPFLAGS) $(SRCDIR)/FindEmbeddedFromClique.cpp $(SRCDIR)/Kruskal.cpp $(SRCDIR)/Prims.cpp -o $(BINDIR)/$@

FindMissingCVEFromClique: $(SRCDIR)/FindMissingCVEFromClique.cpp
	$(CPP) $(CPPFLAGS) $< -o $(BINDIR)/$@

CullMatchesByCount: $(SRCDIR)/CullMatchesByCount.cpp
	$(CPP) $(CPPFLAGS) $< -o $(BINDIR)/$@

CheckEmbedded: $(SRCDIR)/CheckEmbedded.cpp
	$(CPP) $(CPPFLAGS) $< -o $(BINDIR)/$@

StemMatches: $(SRCDIR)/StemMatches.cpp
	$(CPP) $(CPPFLAGS) $< -o $(BINDIR)/$@

clean:
	rm $(ALLBINS)
