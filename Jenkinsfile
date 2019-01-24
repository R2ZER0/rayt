pipeline {
  agent {
    docker {
      image 'gcc'
    }

  }
  stages {
    stage('Build') {
      steps {
        sh 'bash ./build.sh'
      }
    }
  }
}