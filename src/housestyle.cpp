#include "housestyle.h"
#include "readfile.h"

HouseStyle::HouseStyle(
  const std::string& bufferParameter,
  const std::string& ruleDirectoryParameter,
  const std::string& ruleFileParameter,
  const std::string& filterDirectoryParameter,
  const std::string& filterFileParameter,
  const std::string& pathSeparatorParameter,
  int contextRangeParameter) :
  buffer(bufferParameter),
  ruleDirectory(ruleDirectoryParameter),
  ruleFile(ruleFileParameter),
  filterDirectory(filterDirectoryParameter),
  filterFile(filterFileParameter),
  pathSeparator(pathSeparatorParameter),
  contextRange(contextRangeParameter),
  ruleVector(new std::vector<boost::shared_ptr<Rule> >),
  dictionary(new StringSet<char>),
  passiveDictionary(new StringSet<char>)
{
}

HouseStyle::~HouseStyle()
{
}

void HouseStyle::collectFilter(
  std::string& fileName,
  std::set<std::string>& excludeSet,
  int *filterCount)
{
  if (fileName == "(No filter)")
    return;
    
  string filePath, buffer;
  filePath = filterDirectory + pathSeparator + fileName;
  
  if (!ReadFile::run(filePath, buffer))
    return;
  
  XmlFilterReader xfr;
  if (!xfr.parse(buffer))
  {
    std::string report = xfr.getLastError();
    throw runtime_error(report.c_str());
  }
  
  std::map<std::string, std::map<std::string, std::set<std::string> > >
    temporaryMap;
  std::map<std::string, std::map<std::string, std::set<std::string> > >::iterator
    temporaryMapIterator;
  xfr.getFilterMap(temporaryMap);
  
  for (temporaryMapIterator = temporaryMap.begin();
    temporaryMapIterator != temporaryMap.end();
    ++temporaryMapIterator)
  {
    filterMap.insert(*temporaryMapIterator);
    (*filterCount)++;
  }
  
  // add current file to exclude set
  excludeSet.insert(fileName);

  // fetch exclude vector
  std::vector<std::string> localExcludeVector;
  std::vector<std::string>::iterator excludeIterator;
  xfr.getExcludeVector(localExcludeVector);
  for (excludeIterator = localExcludeVector.begin();
    excludeIterator != localExcludeVector.end();
    excludeIterator++)
    excludeSet.insert(*excludeIterator);

  // fetch include vector
  std::vector<std::string> includeVector;
  std::vector<std::string>::iterator includeIterator;
  xfr.getIncludeVector(includeVector);

  if (includeVector.empty())
    return;
    
  for (includeIterator = includeVector.begin();
    includeIterator != includeVector.end();
    includeIterator++)
  {
    if (!excludeSet.count(*includeIterator))
      collectFilter(*includeIterator, excludeSet, filterCount);
  }
}
  
void HouseStyle::collectRules(string& fileName,
  boost::shared_ptr<std::vector<boost::shared_ptr<Rule> > > ruleVector,
  std::set<string>& excludeSet,
  int *ruleCount)
{
  std::string filePath, buffer;
  filePath = ruleDirectory + pathSeparator + fileName;
  if (!ReadFile::run(filePath, buffer))
    return;

  std::auto_ptr<XmlRuleReader> xrr(new XmlRuleReader(
    dictionary,
    passiveDictionary,
    ruleVector));
  if (!xrr->parse(buffer))
  {
    std::string report = xrr->getIncorrectPatternReport();
    if (report != "")
      throw runtime_error(report.c_str());
    else
      throw runtime_error(xrr->getLastError().c_str());
  }

  // add current file to exclude set
  excludeSet.insert(fileName);

  // fetch exclude vector
  std::vector<std::string> localExcludeVector;
  std::vector<std::string>::iterator excludeIterator;
  xrr->getExcludeVector(localExcludeVector);
  for (excludeIterator = localExcludeVector.begin();
    excludeIterator != localExcludeVector.end();
    excludeIterator++)
    excludeSet.insert(*excludeIterator);

  *(ruleCount) += xrr->getRuleCount();

  // fetch include vector
  std::vector<std::string> includeVector;
  xrr->getIncludeVector(includeVector);
  std::vector<std::string>::iterator includeIterator;
  for (includeIterator = includeVector.begin();
    includeIterator != includeVector.end();
    includeIterator++)
  {
    if (!excludeSet.count(*includeIterator))
      collectRules(*includeIterator, ruleVector, excludeSet, ruleCount);
  }
}
  
bool HouseStyle::createReport()
{
  if (!updateRules())
  {
    error = "no rules found";
    return false;
  }

  updateFilter();

  auto_ptr<HouseStyleReader> xtr(new HouseStyleReader(filterMap));
  if (!xtr->parse(buffer))
  {
    error = "file is not well-formed";
    return false;
  }
  std::vector<std::pair<std::string, unsigned> > nodeVector;
  xtr->getNodeVector(nodeVector);
              
  int ruleVectorsize, nodeVectorSize;

  /*
  boost::shared_ptr<Rule> rule(new Rule(
    "_",
    false));
  */

  std::vector<ContextMatch> contextVector;
  std::vector<ContextMatch>::iterator matchIterator;
  ruleVectorsize = ruleVector->size();

  nodeVectorSize = nodeVector.size();
  
  Spellcheck spellcheck(dictionary, passiveDictionary);

  std::string nodeBuffer;
  unsigned elementCount;
  for (int j = 0; j < nodeVectorSize; ++j)
  {
    nodeBuffer = nodeVector.at(j).first;
    elementCount = nodeVector.at(j).second;

    if (!nodeBuffer.size())
       continue;
    
    for (int i = 0; i < ruleVectorsize; i++)
    {
      //rule = ruleVector->at(i);
      boost::shared_ptr<Rule> rule(ruleVector->at(i));
      if (rule->matchPatternGlobal(
        nodeBuffer,
        contextVector,
        elementCount,
        contextRange))
      {
        std::string report = rule->getReport();
    
        for (matchIterator = contextVector.begin();
          matchIterator != contextVector.end();
          matchIterator++)
        {
          if (rule->getAdjustCaseAttribute())
            CaseHandler::adjustCase(
              matchIterator->replace,
              matchIterator->match);

          // tentative?
          matchIterator->tentative =
            (rule->getTentativeAttribute()) ? true : false;
    
          matchIterator->report = report;

          matchVector.push_back(*matchIterator);
        }
        contextVector.clear();
      }
    }
    // check spelling
    if (!dictionary->empty())
    {
      spellcheck.checkString(
        nodeBuffer,
        contextVector,
        contextRange);
      
      for (matchIterator = contextVector.begin();
        matchIterator != contextVector.end();
        matchIterator++)
      {
        matchIterator->report = "Not in dictionary";
        matchIterator->elementCount = elementCount;

        matchVector.push_back(*matchIterator);
      }
      contextVector.clear();
    }  
  }
  return true;
}

std::string HouseStyle::getLastError()
{
  return error;
}

std::vector<ContextMatch> HouseStyle::getMatchVector()
{
  return matchVector;
}

int HouseStyle::updateRules()
{
  ruleVector->clear();
  dictionary->clear();
  passiveDictionary->clear();

  int ruleCount = 0;
  set<string> excludeSet;
  collectRules(ruleFile, ruleVector, excludeSet, &ruleCount);
  return ruleCount;
}

int HouseStyle::updateFilter()
{
  filterMap.clear();
  int filterCount = 0;
  set<string> excludeSet;
  collectFilter(filterFile, excludeSet, &filterCount);

  return filterCount;
}