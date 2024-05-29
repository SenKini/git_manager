#include "github_client.h"

void GithubClient::initRequest(QNetworkRequest &request) {
    request.setRawHeader("Accept", "application/vnd.github+json");
    request.setRawHeader("X-GitHub-Api-Version", "2022-11-28");
    request.setRawHeader("User-Agent", "Qt/6.6");
    request.setRawHeader("Authorization", "Bearer " + _token.toUtf8());
}

QString GithubClient::getInfoUrl() {
    return "https://api.github.com/user";
}

QString GithubClient::getRepoUrl() {
    return "https://api.github.com/user/repos";
}

GithubClient::GithubClient(const QString t)
    : GitClient(t) {
    init();
}
