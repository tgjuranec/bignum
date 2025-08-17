pipeline {
    agent {
        dockerfile true 
    }

    stages {
        stage('Checkout'){
            steps {
		sh 'ls -al'
		sh 'cmake -B build -S .'
            }
        }
        stage('Build') {
            steps {
                sh 'echo "Building..."'
                sh '''
                    cmake --build build
                    '''
            }
        }

        stage('Test') {
            steps {
                sh 'echo "Test..."'
                sh '''
                    cd test && cmake -B build -S .
		    cmake --build build
		    ./build/bignum_test
                    '''
            }
        }
        stage('Deploy') {
            steps {
                sh 'tar -czf libbignum.tar.gz build/libbignum.a'
                archiveArtifacts artifacts: 'libbignum.tar.gz', fingerprint: true
            }
        }
    }
}
